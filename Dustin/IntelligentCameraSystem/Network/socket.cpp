#include <QtGlobal>

#if defined(Q_OS_WIN32)
#include <winsock2.h>
#elif defined(Q_OS_LINUX)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif
#include <errno.h>

#include "Global/Log.h"
#include "Network/socket.h"

Socket::Socket() :
    m_isOpen(false)
{
}
Socket::~Socket()
{
    this->Close();
    INFO() << "Destroyed the socket object.";
}
bool Socket::Open(unsigned short port)
{
    errno = 0;
    //Setting up the windows socket layer
#ifdef Q_OS_WIN32
    WSADATA WsaData;
    if(!(WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR))
    {
        L_ERROR() << "Unable to initialize windows socket layer..." << strerror(errno);
    }
#endif

    //Open a socket
    this->m_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_handle < 0)
    {
        this->m_isOpen = false;
        L_ERROR() << "Could not get handle on socket: " << strerror(errno);
        return false;
    }

    //Bind to a socket on the host
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(m_handle, (const sockaddr *)&address, sizeof(sockaddr_in)) < 0)
    {
        this->m_isOpen = false;
        L_ERROR() << "Could not bind to socket: " << strerror(errno);
        return false;
    }

    //Set socket non-blocking
#if defined(Q_OS_LINUX)
    int nonblocking = 1;
    if(fcntl(m_handle, F_SETFL, O_NONBLOCK, nonblocking) == -1)
    {
        this->m_isOpen = false;
        L_ERROR() << "Could not set socket non-blocking: " << strerror(errno);
        return false;
    }
#elif defined(Q_OS_WIN32)
    DWORD nonblocking = 1;
    if((ioctlsocket(m_handle, FIONBIO, &nonblocking) != 0))
    {
        L_ERROR() << "Could not set non-blocking " << strerror(errno);
    }
#endif
    return true;
}

//Close the socket
void Socket::Close()
{
#if defined(Q_OS_WIN32)
    closesocket(this->m_handle);
    WSACleanup();
#elif defined(Q_OS_LINUX)
    close(this->m_handle);
#endif

    INFO() << "Closed socket: " << this->m_handle;
    this->m_isOpen = false;
}

//Return whether or not socket is open
bool Socket::isOpen() const
{
    return this->m_isOpen;
}

bool Socket::Send(const Address& addr, const void *data, int size)
{
    INFO() << "Sending packet with size: " << size;
    //Set up the destination address
    sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(addr.getAddress().c_str());
    dest.sin_port = htons((unsigned short)addr.getPort());

    int sent_bytes = sendto(this->m_handle, (const char *)data, size, 0, (sockaddr *)&dest, sizeof(sockaddr_in));

    if(sent_bytes != size)
    {
        L_ERROR() << "RETURNED SIZE DOES NOT MATCH SIZE OF PACKET!";
        return false;
    }
    else
    {
        INFO() << "Packet sent sucessfully.";
        if(size < 50)
        {
            INFO() << "Payload: " << (const char *)data;
        }
        return true;
    }

}

//Function to receive data
bool Socket::Receive(Address &addr, void *data, int size)
{
#ifdef Q_OS_WIN32
    typedef int socklen_t;
#endif
    sockaddr_in from;
    socklen_t fromLength = sizeof(from);

    int received_bytes = recvfrom(this->m_handle, (char *) data, size, 0, (sockaddr *)&from, &fromLength);

    if(received_bytes <= 0)
        return false;

    string address(inet_ntoa(from.sin_addr));
    unsigned int port = ntohs(from.sin_port);
    addr.setAddress(address);
    addr.setPort(port);

    return true;
}
