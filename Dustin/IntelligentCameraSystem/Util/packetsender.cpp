

#include "Global/SocketConstants.h"
#include "Global/Log.h"
#include "Network/address.h"
#include "Network/socket.h"
#include "packetsender.h"

PacketSender::PacketSender()
{
}

void PacketSender::run()
{
    Socket sock;
    //Lock this process
    this->m_mutex.lock();
    bool status = sock.Open((unsigned short) SEND_PORT);
    if(status)
    {
        INFO() << "Setting up receiver address..........";
        Address addr(RECEIVER_ADDRESS, RECEIVER_PORT);
        INFO() << "Send data: " << this->m_buffer;
        status = sock.Send(addr, this->m_buffer, strlen(this->m_buffer));

    }
    else
    {
        L_ERROR() << "UNABLE TO OPEN SOCKET.......";
    }
    //Unlock process
    this->m_mutex.unlock();
}


//getBuffer
char *PacketSender::getBuffer() const
{
    return this->m_buffer;
}

//setBuffer
void PacketSender::setBuffer(const char *buffer)
{
    this->m_buffer = (char *)buffer;
}
