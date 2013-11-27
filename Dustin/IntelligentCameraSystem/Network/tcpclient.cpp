#include <QtGlobal>
#if defined(Q_OS_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(Q_OS_LINUX)
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#endif
#include <errno.h>
#include <Global/Log.h>
#include <Global/SocketConstants.h>
#include <Network/address.h>
#include "tcpclient.h"

TCPClient::TCPClient() :
    m_is_open(false),
    m_is_connected(false)
{
}

//Open up the necessary sockets
bool TCPClient::openLocal()
{
    INFO() << "Opening up a stream connection.";
    int status;
#ifdef Q_OS_WIN32
    WSADATA WsaData;
    if(!(WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR))
    {
        L_ERROR() << "Unable to initialize windows socket layer..." << strerror(errno);
    }
#endif

    struct addrinfo description, *servinfo;
    //For setsockopt
#ifdef Q_OS_LINUX
    int set = 1;
#elif defined(Q_OS_WIN32)
    BOOL set = TRUE;
#endif
    //Populate socket description
    memset(&description, 0, sizeof(description));
    description.ai_family = AF_UNSPEC;
    description.ai_socktype = SOCK_STREAM;
    description.ai_flags = AI_PASSIVE;

    //Find local address
    if((status = getaddrinfo(NULL, STR_SEND_PORT, &description, &servinfo)) != 0)
    {
        L_ERROR() << "Unable to find address specified: " << gai_strerror(status);
        return false;
    }
    struct addrinfo *iter;
    //Iterate through to get address
    for(iter = servinfo; iter != NULL; iter = servinfo->ai_next)
    {
        if((m_sockfd_local = socket(iter->ai_family, iter->ai_socktype,
                                    iter->ai_protocol)) == -1)
        {
            //Restart loop if not found
            continue;
        }
#ifdef Q_OS_LINUX
        int sockopt = setsockopt(m_sockfd_local,SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
#elif defined(Q_OS_WIN32)
        int sockopt = setsockopt(m_sockfd_local,SOL_SOCKET, SO_REUSEADDR, (const char *)set, sizeof(int));
#endif
        if(sockopt == -1)
        {
            L_ERROR() << "setsockopt: " << strerror(errno);
            return false;
        }

        //Bind to the socket
        if(bind(m_sockfd_local, iter->ai_addr, iter->ai_addrlen) == -1)
        {
            L_ERROR() << "Could not bind to socket: " << strerror(errno);
#ifdef Q_OS_LINUX
            close(m_sockfd_local);
#elif defined(Q_OS_WIN32)
            closesocket(m_sockfd_local);
#endif
            continue;
        }
        break;
    }

    if(iter == NULL)
    {
        L_ERROR() << "Could not find any sockets.";
        return false;
    }

    //Now we are bound to the right address and port
    freeaddrinfo(servinfo);
    m_is_open = true;
    return true;
}
//Listen for incoming connections
bool TCPClient::Listen()
{
    if(listen(m_sockfd_local, BACKLOG) == -1)
    {
        L_ERROR() << "Could not listen: " << strerror(errno);
        return false;
    }
    return true;
}

TCPError TCPClient::connectToServer(Address server_addr)
{
    int status;
#ifdef Q_OS_WIN32
    WSADATA WsaData;
    if(!(WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR))
    {
        L_ERROR() << "Unable to initialize windows socket layer..." << strerror(errno);
    }
#endif
    struct addrinfo description, *servinfo;
    //For setsockopt
    int set = 1;
    //Populate socket description
    memset(&description, 0, sizeof(description));
    description.ai_family = AF_UNSPEC;
    description.ai_socktype = SOCK_STREAM;

    //Find local address
    if((status = getaddrinfo(server_addr.getAddress().c_str(), server_addr.getStringPort(),
                             &description, &servinfo)) != 0)
    {
        L_ERROR() << "Unable to find address specified: " << gai_strerror(status);
        return TCP_STATUS_ADDRESS_NOT_FOUND;
    }
    struct addrinfo *iter;
    //Iterate through to get address
    for(iter = servinfo; iter != NULL; iter = iter->ai_next)
    {
        if((m_sockfd_server = socket(iter->ai_family, iter->ai_socktype,
                                    iter->ai_protocol)) == -1)
        {
            //Restart loop if not found
            continue;
        }

        //Connect to server
        if(connect(m_sockfd_server, iter->ai_addr, iter->ai_addrlen) == -1)
        {
            L_ERROR() << "Could not bind to socket: " << strerror(errno);
#ifdef Q_OS_LINUX
            close(m_sockfd_server);
#elif defined(Q_OS_WIN32)
            closesocket(m_sockfd_server);
#endif
            continue;
        }
        break;
    }

    if(iter == NULL)
    {
        L_ERROR() << "Could not find any sockets.";
        return TCP_STATUS_SOCKET_NOT_OPENED;
    }

    //Now we are connected to the server
    freeaddrinfo(servinfo);
    m_is_connected = true;
    return TCP_STATUS_SUCCESS;
}
//Disconnect from server
void TCPClient::disconnect()
{
#if defined(Q_OS_WIN32)
    closesocket(m_sockfd_server);
    WSACleanup();
#elif defined(Q_OS_LINUX)
    //close the socket fd
    INFO() << "Closing connection to server.";
    shutdown(m_sockfd_server, 2);
    if(close(m_sockfd_server) == -1)
    {
        L_ERROR() << "Unable to close connection to server!";
    }
#endif


    m_is_connected = false;
}
///Function to send data
TCPError TCPClient::Send(const void *send_data, int size)
{
    //Check if we have a connection
    if(!m_is_connected)
    {
        L_ERROR() << "No open connection!";
        return TCP_STATUS_CONNECTION_CLOSED;
    }
    int bytes_sent = 0;
    //send the data


    do{
#ifdef Q_OS_LINUX
        int sent = send(m_sockfd_server, send_data, size, MSG_DONTWAIT);
#elif defined(Q_OS_WIN32)
        //Windows won't let us set non-blocking...
        int sent = send(m_sockfd_server, (const char *)send_data, size, 0);
#endif
        if(sent == -1)
        {
            L_ERROR() << "Could not send data! " << strerror(errno);
            return TCP_STATUS_SEND_FAILED;
        }
        bytes_sent += sent;
    }while(bytes_sent < size);

    return TCP_STATUS_SUCCESS;
}
//Receive function; returns whether or not data was received
TCPError TCPClient::Receive(char *recv_data, int max)
{
    //See if we are connected
    if(!m_is_connected)
    {
        L_ERROR() << "No open connection to receive from!";
        return TCP_STATUS_CONNECTION_CLOSED;
    }
#ifdef Q_OS_WIN32
    typedef int socklen_t;
#endif
    //Receive data
#ifdef Q_OS_LINUX
    int status = recv(m_sockfd_server, recv_data, max, MSG_DONTWAIT);
#elif defined(Q_OS_WIN32)
    //Windows won't let us make the socket non-blocking....
    int status = recv(m_sockfd_server, recv_data, max, 0);
#endif
    //If error
    if(status == -1)
    {
        L_ERROR() << "Receiving: " << strerror(errno);
        return TCP_STATUS_RECEIVE_FAILED;
    }
    //If not connected
    else if(status == 0)
    {
        WARN() << "Server is disconnected!";
        m_is_connected = false;
        return TCP_STATUS_SERVER_DISCONNECTED;
    }
    return TCP_STATUS_SUCCESS;
}

bool TCPClient::isOpen() const
{
    return this->m_is_open;
}

bool TCPClient::isConnected() const
{
    return this->m_is_connected;
}
