#include <QLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include "Global/Log.h"
#include "videowindow.h"
#include "ui_videowindow.h"

VideoWindow::VideoWindow(Camera *camera, IControlCenterManager *control_center, QWidget *parent) :
    QDialog(parent),
    m_control_center_manager(control_center),
    m_camera(camera),
    ui(new Ui::VideoWindow),
    m_controls_revealed(false),
    m_is_inside(false),
    m_currentbw_kbps(5000),
    m_decision_interface(this),
    m_current_params(),
    m_pending_parameters()
{   

    ui->setupUi(this);
    this->setWindowTitle(m_camera->name());
    //this->setFixedSize(this->size());
    this->setFixedSize(ui->video_player->width() + 10, ui->video_player->height() + ui->video_controls->height() + 5);

    connect(ui->video_player, SIGNAL(resizeParent(int,int)), this, SLOT(resizeWindow(int,int)));
    connect(ui->video_controls, SIGNAL(sendClicked()), this, SLOT(sendButtonClicked()));
    connect(ui->video_controls, SIGNAL(sendClicked(QString,QString,QString, QString)), this, SLOT(resizeVideo(QString,QString,QString, QString)));

    ui->video_controls->resize(this->width(), ui->video_controls->height());
    ui->video_controls->move((this->width() / 2) - (ui->video_controls->width() / 2),0);

    ui->video_player->move((this->width() / 2) - (ui->video_player->width() / 2),ui->video_controls->height());

    //Try to connect to server
    m_tcp_interface = new NetworkToQtInterface(m_camera->server_address());
    connect(m_tcp_interface, SIGNAL(messageDispatch(QString)), this, SLOT(receivedMessage(QString)));
    connect(m_tcp_interface, SIGNAL(serverDisconnected()), this, SLOT(disconnect()));
    //connect(m_request_thread, SIGNAL(destroyed()), m_tcp_interface, SLOT(close()));

    this->setUpThreads();
    //Delay server connect
#ifdef Q_OS_LINUX
    QTimer::singleShot(500, this, SLOT(delayServerConnect()));
#endif



    //qApp->installEventFilter(this);
}

//Set up request/response thread
void VideoWindow::setUpThreads()
{

    //Set up thread
    m_request_thread = new QThread(this);
    m_tcp_interface->moveToThread(m_request_thread);
    //Set up thread connections
    connect(m_request_thread, SIGNAL(started()), m_tcp_interface, SLOT(procRequest()));
    connect(m_tcp_interface, SIGNAL(finished()), m_request_thread, SLOT(quit()));

    //Set up and run bandwidth determination thread
    QString file = m_camera->name() + "_bandwidth";
    m_bwfilereader = new BandwidthFileReader(file);
    m_bwreaderthread = new QThread(this);
    m_bwfilereader->moveToThread(m_bwreaderthread);
    //Set up thread connections
    connect(m_bwreaderthread, SIGNAL(started()), m_bwfilereader, SLOT(readLoop()));
    connect(m_bwfilereader, SIGNAL(finished()), m_bwreaderthread, SLOT(quit()));

    connect(m_bwfilereader, SIGNAL(newBandwidth(QString)), this, SLOT(onBandwidth(QString)));
    //Start the read loop
    m_bwreaderthread->start();

}
//Event filter
//bool VideoWindow::eventFilter(QObject *obj, QEvent *event)
//{
//  if (event->type() == QEvent::MouseMove)// && m_is_inside)
//  {
//    QMouseEvent *e = static_cast<QMouseEvent*>(event);
//    if((e->y() < (this->height() / 4)) && !m_controls_revealed)
//    {
//        ui->video_controls->fadeIn();
//        m_controls_revealed = true;
//    }
//    else if((e->y() > (this->height() / 4)) && m_controls_revealed)
//    {
//        ui->video_controls->fadeOut();
//        m_controls_revealed = false;
//    }
//    //statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
//  }
//  return false;
//}


VideoWindow::~VideoWindow()
{
    delete ui;
    INFO() << "Video window destructor called.";
    //Must determine why server is not seeing us disconnect
    this->disconnect();
    //ui->video_player->stopPlaying();


    m_request_thread->terminate();


    delete m_tcp_interface;

    delete m_request_thread;

    m_bwfilereader->setReadyFlag(false);

    m_bwreaderthread->terminate();

    delete m_bwfilereader;
    delete m_bwreaderthread;

    //Camera object should be managed by creating class
    //delete m_camera;
    emit sendCamera(m_camera);



}

void VideoWindow::streamVideo(QString url)
{
#if PLAY_WITH_VLC
   ui->video_player->playUrl(url);
    //ui->video_player->preprocess();
#endif
}

void VideoWindow::enterEvent(QEvent *e)
{
//    QDialog::enterEvent(e);
//    m_is_inside = true;
}

void VideoWindow::leaveEvent(QEvent *e)
{
//    QDialog::leaveEvent(e);
//    m_is_inside = false;
//    //Make sure we hide the controls if revealed
//    if(m_controls_revealed)
//    {
//        ui->video_controls->fadeOut();
//        m_controls_revealed = false;
//    }
}



void VideoWindow::resizeWindow(int width, int height)
{
    //this->setFixedSize(width, height);
    this->setFixedSize(width + 10, height + ui->video_controls->height() + 5);
    ui->video_controls->resize(this->width(), ui->video_controls->height());
    ui->video_controls->move((this->width() / 2) - (ui->video_controls->width() / 2),0);
    ui->video_player->move((this->width() / 2) - (ui->video_player->width() / 2),ui->video_controls->height());
}

//Slot to handle packet data received
void VideoWindow::receivedMessage(QString response)
{
    INFO() << "In slot \'receivedPacket\' Message:" << response;
    //Set non modal and allocate on heap so it does not block
    //video from playing
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setWindowTitle("Server says...");
    msgBox->setText(response);
    msgBox->setWindowModality(Qt::NonModal);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->show();

    //Check if the response is an acceptance of the parameters
    if(response.contains("Updating"))
    {
        m_current_params = m_pending_parameters;

        ui->video_player->resetStats();

        //If we are in learning mode
        if(m_control_center_manager->inLearningMode())
        {
            //Add this to the training set
        }
    }
}

void VideoWindow::delayServerConnect()
{
    connectToServer();
}

void VideoWindow::connectToServer()
{
    //Initial connection attempt
    bool connected = m_tcp_interface->Connect();
#if CONNECTION_MULTIPLE_ATTEMPTS
    if(!connected)
    {
#ifdef Q_OS_LINUX
        int msecs = 100;
        int total_attempts = 50;
#elif defined(Q_OS_WIN32)
        int msecs = 10;
        int total_attempts = 5;
#endif
        //Set up a progress dialog
        QProgressDialog progress("Connecting", "Abort", 0, total_attempts-1, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();
        QThread::msleep(100);
        //Make an empty progress bar
        QProgressBar bar;
        progress.setBar(&bar);
        bar.setMinimum(0);
        bar.setMaximum(0);

        int attempts = 1;

        //Keep trying to connect for 5 seconds
        while(!connected && attempts < total_attempts)
        {
            //Update label every second
            if(attempts % 10 == 0)
            {
                progress.setLabelText(progress.labelText().append("."));
            }
            //Update attempt number
            progress.setValue(attempts);
            //Sleep for 1/10 of a second
            QThread::msleep(msecs);
            connected = m_tcp_interface->Connect();
            attempts++;
            //Break when we are done attempting
            if(progress.wasCanceled())
                break;
        }
        progress.hide();
    }
#endif
    if(connected)
    {
        receivedMessage("Connected!");
        //Start playing video
 #if PLAY_WITH_VLC
        this->ui->video_player->playUrl(QString(VIDEO_URL));
 #else
        this->ui->video_player->play();
 #endif

    }
    else
    {
        receivedMessage("Unable to connect!");

    }
}

//Function to send a request to the server
void VideoWindow::sendRequest(QString data)
{
    if(!m_tcp_interface->isConnected())
        receivedMessage("Not connected!");
    else
    {
        m_tcp_interface->setBuffer(data);
        m_request_thread->start();
    }
}


Camera *VideoWindow::camera() const
{
    return m_camera;
}

void VideoWindow::setCamera(Camera *camera)
{
    m_camera = camera;
}

bool VideoWindow::operator ==(const VideoWindow &rhs)
{
    return *(this->camera()) == *(rhs.camera());
}

QStringList VideoWindow::getVideoSizes()
{
    return this->ui->video_controls->getSizes();
}
//Function to disconnect from camera server
void VideoWindow::disconnect()
{
    m_tcp_interface->close();
    
}

void VideoWindow::sendButtonClicked()
{
    //bool playing = this->ui->video_player->isPlaying();
    QString str;
    //if(playing)
        str = "Playing";
    //else
        //str = "Not Playing";
    receivedMessage(str);
}
//Slot to send message to server to resize the video
void VideoWindow::resizeVideo(QString width, QString height, QString fps, QString bps)
{
    //Create the message
    QString msg = QString("start ");
    msg += width + QString(" ") + height + QString(" ") + fps + QString(" ") + bps;
    INFO() << "Sent: " << msg;
    //Send it to server
    this->sendRequest(msg);


#if PLAY_WITH_VLC
    int fps_int = fps.toInt();
    //If FPS has changed we need to restart playback (limit to 15 and 30 fps for now... very poor and hacky method)
    if(fps_int != ui->video_player->getFps() && (fps_int == 30 || fps_int == 15))
    {
        ui->video_player->setFps(fps_int);
        //ui->video_player->setPlaybackRate(fps.toInt());
        this->ui->video_player->playUrl(QString(VIDEO_URL));
    }
#endif

    //Set new encoding parameters
    EncodingParameters next(width, height, fps, bps);
    if(next.bitrateAsInt() == 0)
    {
        next.setBitrate(QString::number(next.widthAsInt() * next.heightAsInt() * 3) + (next.bitrate().endsWith('\'') ? "'" : ""));
    }
    m_pending_parameters = next;

}

//Change the current bandwidth of the channel
void VideoWindow::onBandwidth(QString bandwidth)
{
    bool success = false;

    int iBW = bandwidth.toInt(&success, 10);

    if(success)
    {
        this->m_currentbw_kbps = iBW;

        //Are we in learning mode?
        if(m_control_center_manager->inLearningMode())
        {
            //Add training example to training set and determine the decision function
        }
        else
        {
            //Determine the new encoding parameters
            EncodingParameters new_params;
            float dr_avg_kbps = ui->video_player->getAverageBitrate() * 10000;
            FeatureSet fs;
            m_decision_interface.makeDecision(m_currentbw_kbps, dr_avg_kbps, m_current_params, fs, new_params);

            this->resizeVideo(new_params.width(), new_params.height(), new_params.fps(), new_params.bitrate());

        }
    }
}

