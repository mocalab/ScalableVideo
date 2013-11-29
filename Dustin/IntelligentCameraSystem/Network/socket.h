/**
 *  @file Definition of a basic C++ wrapper to a UDP socket.
 */
#ifndef SOCKET_H
#define SOCKET_H

#include "Network/address.h"

class Socket
{
public:
    Socket();
    ~Socket();

    bool Open(unsigned short port);
    void Close();

    bool isOpen() const;
    bool Send(const Address& addr, const void *data, int size);
    bool Receive(Address &addr, void *data, int size);
private:
    int                 m_handle;
    bool                m_isOpen;
};

#endif // SOCKET_H
