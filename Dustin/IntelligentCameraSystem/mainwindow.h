#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <UI/serverpropertiesdialog.h>
#include <QThread>
#include <list>

//#include "Util/packetsender.h"
//#include "Util/packetreceiver.h"
//#include "Util/messagedispatcher.h"
#include "Network/networktoqtinterface.h"
#include "Network/address.h"
#include <UI/videowindow.h>
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void delayServerConnect();
    void on_bConnect_clicked();

public slots:
    void on_bSend_clicked();
    void receivedMessage(const char *);
    void receivedMessage(QString response);
    //Function to disconnect from server
    void disconnect();
    //Slot to open the server properties dialog box
    void openServerProperties();


private:
    //Data members
    Ui::MainWindow *ui;
    ServerPropertiesDialog *server_properties;



    //NEW -- TCP Client
    NetworkToQtInterface    *m_tcp_interface;
    //TCP request/response thread
    QThread                 *m_request_thread;
    //Server address
    Address                 m_server_address;
    //Functions
    VideoWindow *vid;

    //Function to connect to server
    void connectToServer();


    //Function to send request
    void sendRequest(QString data);

    //Set up threads
    void setUpThreads();

    //THESE MEMBERS ARE NO LONGER USED BUT LEFT JUST IN CASE
    //WE WISH TO USE UDP
//    Thread to be dynamically allocated
//    PacketSender            *sender_thread;
//    Another Thread to receive packets
//    PacketReceiver          *receiver_thread;
//    MessageDispatcher        m_messageDispatcher;

//    Function to send a UDP packet
//    int sendPacket(const char* data);

//    Utility function to receive a UDP packet
//    int listenForPackets();

};

#endif // MAINWINDOW_H
