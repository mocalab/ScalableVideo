#include <QtGlobal>
#include <QMessageBox>
#include <QProgressDialog>
#include <QProgressBar>
#include <QTimer>
#include <exception>
#include <string>
#include "Global/Log.h"
#include "mainwindow.h"
#include "Global/SocketTypes.h"
#include "ui_mainwindow.h"
#include "Util/messagedispatcher.h"
#include "UI/videowindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->teCommand, SIGNAL(enterPressed()), this, SLOT(on_bSend_clicked()));
    this->server_properties = new ServerPropertiesDialog(this);

    ui->teCommand->selectAll();
    connect(ui->actionServer_Properties, SIGNAL(triggered()), this, SLOT(openServerProperties()));

    m_server_address.setAddress(server_properties->getHostName().toStdString());
    m_server_address.setPort(server_properties->getPort());
    //Try to set up the network connection
    m_tcp_interface = new NetworkToQtInterface(m_server_address);
    connect(m_tcp_interface, SIGNAL(messageDispatch(QString)), this, SLOT(receivedMessage(QString)));
    connect(m_tcp_interface, SIGNAL(serverDisconnected()), this, SLOT(disconnect()));
    this->setUpThreads();
    //Delay server connect
#ifdef Q_OS_LINUX
    //QTimer::singleShot(500, this, SLOT(delayServerConnect()));
#endif
    QString q("Camera A");

    vid = new VideoWindow(new Camera(q, m_server_address, CONTENT_TYPE_PERSONAL));
    vid->setAttribute(Qt::WA_DeleteOnClose);
    vid->show();
    //FOR UDP
    //Set up a packet sender thread
    //this->sender_thread = new PacketSender();
    //Set up a packet receiver thread and execute for the life of the main window
    //this->receiver_thread = new PacketReceiver(&m_messageDispatcher);
    //this->receiver_thread->start();
    //Connect receivers received signal to our slot
    //connect(&m_messageDispatcher, SIGNAL(passMessage(const char *)), this, SLOT(receivedMessage(const char *)));
}

MainWindow::~MainWindow()
{
    delete ui;
    m_request_thread->terminate();
    delete m_tcp_interface;

    delete m_request_thread;
    delete server_properties;


    //For UDP
    //delete sender_thread;
    //receiver_thread->waitToFinish();
    //delete receiver_thread;

}

//Set up request/response thread
void MainWindow::setUpThreads()
{

    //Set up thread
    m_request_thread = new QThread(this);
    m_tcp_interface->moveToThread(m_request_thread);
    //Set up thread connections
    connect(m_request_thread, SIGNAL(started()), m_tcp_interface, SLOT(procRequest()));
    connect(m_tcp_interface, SIGNAL(finished()), m_request_thread, SLOT(quit()));
}

void MainWindow::on_bSend_clicked()
{
    INFO() << "Send button clicked.";
    //Check to see if conencted to server
//    if(!m_tcp_interface->isConnected())
//    {
//        this->receivedMessage("Not connected to the server");
//        return;
//    }
    //Send the command in the text box
    QString cmd = ui->teCommand->toPlainText();
    //Send packet with text edit string as const char *
    std::string c_cmd = cmd.toStdString();

    //Add command to list of previous commands
    if(c_cmd.compare("") != 0)
    {
        ui->teCommand->addCommandToList(c_cmd);
    }

    ui->teCommand->setText("");

    //ui->video_container->playUrl(cmd);
    vid->streamVideo(cmd);

    //cmd = cmd.trimmed();
    if(!cmd.isEmpty())
        this->sendRequest(cmd);

    //FOR UDP

    //Send packet last to protect data integrity
    //this->sendPacket(c_cmd.c_str());
    //Receive any responses from the other end
    //this->listenForPackets();

}

void MainWindow::delayServerConnect()
{
    connectToServer();
}

//Slot to handle packet data received
void MainWindow::receivedMessage(const char *data)
{
    INFO() << "In slot \'receivedPacket\' Message:" << data;
    QMessageBox msgBox;
    msgBox.setText(QString(data));
    int ret = msgBox.exec();
}

//Slot to handle packet data received
void MainWindow::receivedMessage(QString response)
{
    INFO() << "In slot \'receivedPacket\' Message:" << response;
    QMessageBox msgBox;
    msgBox.setText(response);
    int ret = msgBox.exec();
}


//Open the server properties
void MainWindow::openServerProperties()
{
    server_properties->show();
    int ret = server_properties->exec();
    //See what the return value is
    switch(ret)
    {
    case QDialog::Accepted:
        //Update the address info
        m_server_address.setAddress(server_properties->getHostName().toStdString());
        m_server_address.setPort(server_properties->getPort());
        m_tcp_interface->setServerAddress(m_server_address);
        break;
    case QDialog::Rejected:
        break;
    }
}

//Function to attempt to connect to a server
void MainWindow::connectToServer()
{
    //Initial connection attempt
    bool connected = m_tcp_interface->Connect();
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

        //Make an empty progress bar
        QProgressBar bar;
        progress.setBar(&bar);
        bar.setMinimum(0);
        bar.setMaximum(0);

        int attempts = 1;
        progress.show();
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
    if(connected)
    {
        receivedMessage("Connected!");
        this->ui->bConnect->setText("Disconnect");
    }
    else
    {
        receivedMessage("Unable to connect!");
        this->ui->bConnect->setText("Connect");
    }
//    if(connected)
//    {
//        msgbox.setText("Success!");
//    }
//    else{
//        msgbox.setText("Failed!");
    //    }
}

void MainWindow::disconnect()
{
    m_tcp_interface->close();
    this->ui->bConnect->setText("Connect");
}

void MainWindow::sendRequest(QString data)
{
    m_tcp_interface->setBuffer(data);
    m_request_thread->start();
}

//Connect button clicked handler
void MainWindow::on_bConnect_clicked()
{
    if(m_tcp_interface->isConnected())
    {
        this->disconnect();
    }
    else
    {
        connectToServer();
    }

}

//FOR UDP
//int MainWindow::sendPacket(const char *data)
//{
//    //Delete thread if currently exists
//    if(this->sender_thread != NULL)
//        delete this->sender_thread;

//    try{
//        this->sender_thread = new PacketSender();
//        this->sender_thread->setBuffer(data);
//        this->sender_thread->start();
//    }catch(std::bad_alloc& ex)
//    {
//        ERROR() << "Unable to allocate memory: " << ex.what();
//    }
//    catch(std::exception& ex)
//    {
//        ERROR() << "Exception: " << ex.what();
//    }
//    catch(...)
//    {
//        ERROR() << "Unknown error ocurred.";
//    }


//    return 0;
//}
//FOR UDP
//int MainWindow::listenForPackets()
//{
//    INFO() << "Listening for any responses from the android app";
//    if(this->receiver_thread != NULL)
//        delete this->receiver_thread;

//    try{
//        this->receiver_thread = new PacketReceiver(&m_messageDispatcher);
//        this->receiver_thread->start();

//    }catch(std::bad_alloc& ex)
//    {
//        ERROR() << "Unable to allocate memory: " << ex.what();
//    }
//    catch(std::exception& ex)
//    {
//        ERROR() << "Exception: " << ex.what();
//    }
//    catch(...)
//    {
//        ERROR() << "Unknown error ocurred.";
//    }
//    return 0;
//}
