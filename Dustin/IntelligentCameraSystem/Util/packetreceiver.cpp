#include "packetreceiver.h"
#include "Network/socket.h"
#include "Network/address.h"
#include "Global/Log.h"
#include "Global/SocketConstants.h"
#include "messagedispatcher.h"
#include <string>
PacketReceiver::PacketReceiver(MessageDispatcher * passer) :
    m_dispatcher(passer),
    valid(true),
    finishing(false)
{
}

PacketReceiver::~PacketReceiver()
{
    delete data;
}

void PacketReceiver::waitToFinish()
{
    //Finish this thread
    finishing = true;
    while(finishing)
    {
        this->valid = false;
    }
}

void PacketReceiver::run()
{
    Socket sock;
    bool status = sock.Open((unsigned short) LISTEN_PORT);
    //Delegate string to dispatch to main window
    std::string message_delegate;
    if(status)
    {
        data = new char[256];
        Address fromAddress;
        INFO() << "Beginning packet reception loop";
        bool received = false;
        //Begin loop to receive packet
        while(valid)
        {
            if(data == NULL)
                data = new char[256];
           received = sock.Receive(fromAddress, (void *)data, strlen(data));

            if(received)
            {
                INFO() << "Received packet, emitting signal. Payload:" << data;
                //Give the data to a delegate so we can delete the pointer
                message_delegate.assign(data, strlen((const char *)data));
                this->m_dispatcher->reemitMessage(message_delegate.data());
                INFO() << "After signal emit";
                //Delete data and set to null
                delete data;
                data = NULL;
            }
        }
        finishing = false;

    }
}
