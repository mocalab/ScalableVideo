#include <QApplication>
#include <QTimer>
#include <QThread>
#include <Global/Log.h>
#include "networktoqtinterface.h"
#include "address.h"
NetworkToQtInterface::NetworkToQtInterface() :
    m_connection_timeout(false),
    m_buffer("Buffer")
{

}

NetworkToQtInterface::NetworkToQtInterface(Address server_address) :
    m_server_address(server_address),
    m_connection_timeout(false),
    m_buffer("Buffer")
{

}

NetworkToQtInterface::~NetworkToQtInterface()
{
    //Delete members
    //delete m_response_loop;
}

void NetworkToQtInterface::setUpTimer()
{
    INFO() << "Setting up timer...";
    this->m_response_loop = new TickingTimer(this);
    connect(m_response_loop, SIGNAL(timeout()), this, SLOT(listenForResponse()));
}

//Connect function which will use internally defined address
bool NetworkToQtInterface::Connect()
{
    //m_connection_timeout = false;
    //Start a one shot timer to timeout after 10 seconds
   // QTimer::singleShot(1000, this, SLOT(connectionTimeout()));
    //Attempt to connect
    TCPError connected = m_tcp_client.connectToServer(m_server_address);

//    if(connected == TCP_STATUS_SUCCESS)
//    {
//        emit messageDispatch("Connected!");
//    }
//    else
//    {
//        emit messageDispatch("Unable to connect!");
//    }
    return connected == TCP_STATUS_SUCCESS;
}

void NetworkToQtInterface::close()
{
    //Close the connection
    m_tcp_client.disconnect();
    //emit messageDispatch("Disconnected from server!");
}
//Function to send a request to the server and wait for a response
void NetworkToQtInterface::procRequest()
{
    TCPError status = TCP_STATUS_SUCCESS;
    //First, send the request
    m_buffer.append('\n');
    const char *data = m_buffer.toStdString().c_str();
    //INFO() << "Just before send... Data:" << data;
    status = m_tcp_client.Send(data, strlen(data));
    INFO() << "After send. STATUS: " << status;
    //Handle error
    //check for a request for 5 seconds
    //Set up timer
    this->setUpTimer();
    this->m_response_loop->startTicking(500);
}

void NetworkToQtInterface::connectionTimeout()
{
    m_connection_timeout = true;
}


//Function to listen for response messages
void NetworkToQtInterface::listenForResponse()
{
    INFO() << "Response tick...";
    //Get the response
    char response[128];
    TCPError status = m_tcp_client.Receive(response, 128);
    //If we received a message
    if(status == TCP_STATUS_SUCCESS)
    {
        INFO() << "Received response:" << response;
        m_response_loop->stop();
        QString resp(response);
        resp.append('\0');
        this->messageDispatch(resp);
        delete m_response_loop;
        emit finished();
    }
    //If the server disconnected
    else if(status == TCP_STATUS_SERVER_DISCONNECTED)
    {
        INFO() << "Server disconnected";
        m_response_loop->stop();
        this->messageDispatch("Server disconnected!");
        //Close the server connection
        this->close();
        delete m_response_loop;
        emit serverDisconnected();
        emit finished();
    }
    //If we have reached the tick limit
    else if(m_response_loop->getTicks() >= 10)
    {
        INFO() << "Timed out";
        m_response_loop->stop();
        this->messageDispatch("Failed to get a response from the server.");
        delete m_response_loop;
        emit finished();
    }
}


bool NetworkToQtInterface::isConnected() const
{
    return m_tcp_client.isConnected();
}

void NetworkToQtInterface::setBuffer(QString& data)
{
    m_buffer = data;
}

void NetworkToQtInterface::setServerAddress(Address &addr)
{
    this->m_server_address = addr;
}

void NetworkToQtInterface::sendMessage(const QString &msg)
{
    if(m_tcp_client.isConnected())
        m_tcp_client.Send(msg.toStdString().c_str(), msg.size());
}
