#include <QLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QDate>
#include <QTime>
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
    m_currentbw_kbps(0),
    m_decision_interface(this),
    m_current_params(),
    m_pending_parameters(),
    m_bandwidth_filereader(QString(BWFILE_PATH).append(QString(m_camera->name())).append("_bandwidth")),
    m_effective_rate(400),
    m_ctrl_key_down(false),
    m_s_key_down(false)
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
    connect(m_tcp_interface, SIGNAL(messageDispatch(QString, bool)), this, SLOT(receivedMessage(QString, bool)));
    connect(m_tcp_interface, SIGNAL(serverDisconnected()), this, SLOT(disconnect()));
    //connect(m_request_thread, SIGNAL(destroyed()), m_tcp_interface, SLOT(close()));

    this->setUpThreads();
    //Delay server connect
#ifdef Q_OS_LINUX
    QTimer::singleShot(500, this, SLOT(delayServerConnect()));
#endif

    //Set up the timer
//    m_poller = new QTimer(this);
//    connect(m_poller, SIGNAL(timeout()), this, SLOT(takeSample()));
//    m_poller->start(2000);
    //qApp->installEventFilter(this);

    m_bw_file_poller = new QTimer(this);
    connect(m_bw_file_poller, SIGNAL(timeout()), this, SLOT(pollBandwidthFile()));
    this->m_bw_file_poller->start(500);

    //Initialize feature set values
    m_video_features.setContentType(this->m_camera->content_type());

    m_decision_interface.setResolutionsList(ui->video_controls->getSizes());
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
//    QString file = m_camera->name() + "_bandwidth";
//    m_bwfilereader = new BandwidthFileReader(file);
//    m_bwreaderthread = new QThread(this);
//    m_bwfilereader->moveToThread(m_bwreaderthread);
//    //Set up thread connections
//    connect(m_bwreaderthread, SIGNAL(started()), m_bwfilereader, SLOT(readLoop()));
//    connect(m_bwfilereader, SIGNAL(finished()), m_bwreaderthread, SLOT(quit()));

//    connect(m_bwfilereader, SIGNAL(newBandwidth(QString)), this, SLOT(onBandwidth(QString)));
//    //Start the read loop
//    m_bwreaderthread->start();

}
//Ask the user if they find the video acceptable
void VideoWindow::pollUser()
{
    //Their response
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Feedback", "Video quality acceptable?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        this->takeSample();
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

    this->disconnect();
    //ui->video_player->stopPlaying();


    m_request_thread->terminate();


    delete m_tcp_interface;

    delete m_request_thread;

//    m_bwfilereader->setReadyFlag(false);
//    m_bwreaderthread->wait();
//    m_bwreaderthread->terminate();

//    delete m_bwfilereader;
//    delete m_bwreaderthread;

    //delete m_poller;

    m_bandwidth_filereader.close();

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

void VideoWindow::keyReleaseEvent(QKeyEvent *e)
{
    //Take a screenshot if the key entered was tab
    if((m_ctrl_key_down && e->key() == Qt::Key_S) || (e->key() == Qt::Key_Control && m_s_key_down))
    {
        takeScreenshot();
    }

}

void VideoWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control)
    {
        m_ctrl_key_down = true;
    }
    else if(e->key() == Qt::Key_S && m_ctrl_key_down)
    {
        m_s_key_down = true;
    }

}

void VideoWindow::takeScreenshot()
{
    //Get the file path
    QByteArray pathname = qgetenv("INTELLIGENT_CAMERA_SYSTEM_ROOT_DIR");
    QString path(pathname);
    path = path + "/" + SCREENSHOT_PATH + this->m_camera->name() +
            "_" + QDate::currentDate().toString() + "-" + QTime::currentTime().toString() + ".png";
    path = path.simplified().replace(" ", "-");
    bool ret = this->ui->video_player->takeSnapshot(path);

    m_s_key_down = false;
    m_ctrl_key_down = false;
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
void VideoWindow::receivedMessage(QString response, bool show_message)
{
    INFO() << "In slot \'receivedPacket\' Message:" << response;
    //Set non modal and allocate on heap so it does not block
    //video from playing


    //Check if the response is an acceptance of the parameters
    if(response.contains("Updating"))
    {
        //Set the new effective data rate by taking ratios of the new params to old params
        float rate = m_effective_rate;
        rate *= (((float)m_pending_parameters.fpsAsInt() / m_current_params.fpsAsInt()) * ((float)m_pending_parameters.bitrateAsInt() / m_current_params.bitrateAsInt()));
        m_effective_rate = (int)rate;
        DEBUG() << "Effective bitrate: " << m_effective_rate;
        m_current_params = m_pending_parameters;

        ui->video_player->resetStats();

        //If we are in learning mode
        if(m_control_center_manager->inLearningMode())
        {
            //See if the user wants to add this to the training set
            this->pollUser();
        }
    }
    else if(show_message)
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setWindowTitle("Server says...");
        msgBox->setText(response);
        msgBox->setWindowModality(Qt::NonModal);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->show();
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
void VideoWindow::sendRequest(QString data, bool show_message)
{
    if(!m_tcp_interface->isConnected())
        receivedMessage("Not connected!");
    else
    {
        m_tcp_interface->setBuffer(data);
        m_tcp_interface->setShowMessage(show_message);
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
    //Take a training sample
    this->takeSample();

}
//Slot to send message to server to resize the video
void VideoWindow::resizeVideo(QString width, QString height, QString fps, QString bps, bool show_message)
{
    //Take a training sample
    //this->takeSample();
    //Create the message
    QString msg = QString("start ");
    //See if we should change the bitrate
    if(bps == "0" && m_currentbw_kbps != 0)
    {
        //Get the old bitrate
        float old_bitrate = (float)m_current_params.bitrateAsInt();
        int bitrate = 0;
        //Figure out how to fit the channel
        float fps_ratio = (float)fps.toInt() / m_current_params.fpsAsInt();

        //Bitrate will be updated as such due to framerate
        float ratio = m_effective_rate * fps_ratio;

        //Get the ratio of new datarate to channel bandwidth
        ratio = ratio / m_currentbw_kbps;

        //Find the maximum and optimal bitrates
        int max_bitrate = (int)((float)width.toInt() * (float)height.toInt() * 3.5);
        int opt_bitrate = (int)((old_bitrate / ratio) * 0.85);

        //Choose the least of these
        bitrate = max_bitrate < opt_bitrate ? max_bitrate : opt_bitrate;

        bps = QString::number(bitrate);


    }
    //Set new encoding parameters
    EncodingParameters next(width, height, fps, bps);
    if(next.bitrateAsInt() == 0)
    {
        next.setBitrate(QString::number(next.widthAsInt() * next.heightAsInt() * 3) + (next.bitrate().endsWith('\'') ? "'" : ""));
    }
    m_pending_parameters = next;

    msg += width + QString(" ") + height + QString(" ") + fps + QString(" ") + bps;
    INFO() << "Sent: " << msg;


#if PLAY_WITH_VLC
    int fps_int = fps.toInt();
    //If FPS has changed we need to restart playback (limit to 15 and 30 fps for now... very poor and hacky method)
    if(fps_int != ui->video_player->getFps() && (fps_int == 30 || fps_int == 15))
    {
        ui->video_player->setFps(fps_int);
        //ui->video_player->setPlaybackRate(fps.toInt());
        if(fps_int == 30)
            this->ui->video_player->playUrl(QString(VIDEO_URL));
        else
            this->ui->video_player->setMediaOptions();
    }
#endif

    //Send it to server
    this->sendRequest(msg, show_message);






}

//Change the current bandwidth of the channel
void VideoWindow::onBandwidth(QString bandwidth)
{
    bool success = false;

    int iBW = bandwidth.toInt(&success, 10);

    if(success)
    {
        EncodingParameters new_params = m_current_params;
        //Wait until the pending parameters and current parameters are equal
        if(!(m_pending_parameters == m_current_params))
            new_params = m_pending_parameters;

        m_effective_rate = (int)(m_effective_rate == 0 ? ui->video_player->getAverageBitrate() * 10000 : m_effective_rate);
        //Set the new bandwidth for the feature set
        m_video_features.setBandwidth(iBW);
        //Are we in learning mode?
        if(m_control_center_manager->inLearningMode())
        {

            //this->takeSample();
            this->m_currentbw_kbps = iBW;
            //Knee-jerk reaction
            m_decision_interface.defaultAdjustBitrate(m_currentbw_kbps, m_effective_rate, m_current_params, new_params);
        }
        //USE ML ALGORITHM TO DETERMINE WHAT THE PARAMETERS SHOULD BE
        else
        {
            this->m_currentbw_kbps = iBW;
            //Determine the new encoding parameters
            FeatureSet fs;
            //Predict what the user will prefer
            int class_mask = this->m_control_center_manager->predict(this->m_video_features);

            DEBUG() << "Class mask value: " << class_mask;

            m_decision_interface.makeDecision(m_currentbw_kbps, m_effective_rate, m_current_params, class_mask, new_params);
        }
        //m_effective_rate = (float)dr_avg_kbps;
        //DEBUG() << m_effective_rate;
        this->resizeVideo(new_params.width(), new_params.height(), new_params.fps(), new_params.bitrate(), false);
    }
}

//Take a sample and add it to the training set
void VideoWindow::takeSample()
{
    if(m_control_center_manager->inLearningMode())
    {
        //Add training example to training set to determine the decision function

        //Create the feature set object
        FeatureSet next_fs(m_currentbw_kbps * 1000, (int)this->m_camera->content_type());
        //Get the labels

        //We use simple implicit feedback to determine the two classes.

        //If the video is playing at framerate 15fps and enhanced bitrate, the user prefers bitrate over framerate (+1 class)
        //If the video is playing at framerate 30fps and lower bitrate, the user prefers temporal quality over bitrate (-1 class)
        double lbl_fps_bitrate = 0.0;
        if(ui->video_player->getFps() < 30)
        {
            lbl_fps_bitrate = ML_USER_PREFERS_BITRATE;
        }
        else
        {
            lbl_fps_bitrate = ML_USER_PREFERS_FPS;
        }

        //To determine the label for size over quality, we have to look at available bandwidth and the bandwidth of
        //the video. If there is a significant enough difference between the available bandwidth and the data rate of
        //the video the user most likely prefers quality to size. In order to confirm this we will look at the dimensions of
        //the video and check if the current encoding bitrate constitutes high quality video. This will be defined as the +1
        //class. If the data rate is at the maximum that the bandwidth will allow, we will look at the video size, and if the
        //dimensions are large enough, then the user most likely prefers size over video quality. This will be defined as the -1
        //class.
        double lbl_size_quality = 0.0;
        //For now, a trivial solution; will determine using empricial evidence
        if((float)m_current_params.bitrateAsInt() > (float)ui->video_player->width() * ui->video_player->height() * 1.75
                && ui->video_player->width() < 720)
        {
            //Preference to quality
            lbl_size_quality = ML_USER_PREFERS_QUALITY;
        }
        else
        {
            lbl_size_quality = ML_USER_PREFERS_SIZE;
        }

        //Add this new training example
        this->m_control_center_manager->addTrainingExample(next_fs, lbl_fps_bitrate, lbl_size_quality);
    }

}

//TO TEST BANDWIDTH ---
//Use the test script created, specify the dummynet pipe and tick interval, redirect output to the bandwidth file for this camera.
//MAKE SURE TO DELETE THE BANDWIDTH FILE AFTER TESTING!!! OTHERWISE THIS WILL READ EVERY LINE IN THAT FILE AND WILL
//SCREW UP RESULTS!!!
void VideoWindow::pollBandwidthFile()
{
    if(!m_bandwidth_filereader.isOpen())
    {
        //Attempt to open the file
        m_bandwidth_filereader.open_file();
    }
    else
    {
        //Read the next line
        QString line = m_bandwidth_filereader.readLine();
        if(line != QString(""))
        {
            //Indicate new bandwidth
            this->onBandwidth(line);

        }
    }
}

