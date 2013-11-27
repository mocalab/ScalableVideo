#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include <QThread>

class MessageDispatcher;
class PacketReceiver : public QThread
{
    Q_OBJECT
public:
    PacketReceiver(MessageDispatcher * passer);
    ~PacketReceiver();
    void waitToFinish();
protected:
    void run();
private:
    MessageDispatcher   *m_dispatcher;
    char                *data;
    bool                valid;
    bool                finishing;
};

#endif // PACKETRECEIVER_H
