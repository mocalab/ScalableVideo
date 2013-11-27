#ifndef TCPCLIENT_H
#define TCPCLIENT_H

class Address;

//enumeration for any errors
enum TCPError
{
    TCP_STATUS_SUCCESS = 0,
    TCP_STATUS_ADDRESS_NOT_FOUND = 1,
    TCP_STATUS_SOCKET_NOT_OPENED = 2,
    TCP_STATUS_CONNECTION_CLOSED = 3,
    TCP_STATUS_SEND_FAILED = 4,
    TCP_STATUS_RECEIVE_FAILED = 5,
    TCP_STATUS_SERVER_DISCONNECTED = 6
};

class TCPClient
{
public:
    TCPClient();

    //Connect to the server
    TCPError connectToServer(Address server_addr);

    //Disconnect from server
    void disconnect();

    ///Send function
    TCPError Send(const void *send_data, int size);

    //Receive function
    TCPError Receive(char *recv_data, int max);

    //Check if socket is open
    bool isOpen() const;
    bool isConnected() const;

    //For implementing a server (if needed)

    //Open a socket
    bool openLocal();

    //Listen for incoming connections
    bool Listen();

private:
    //Socket file descriptors
    int         m_sockfd_local;
    int         m_sockfd_server;
    //flags
    bool        m_is_open;
    bool        m_is_connected;
};

#endif // TCPCLIENT_H
