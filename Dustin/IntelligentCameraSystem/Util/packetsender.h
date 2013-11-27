#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include <QThread>
#include <QMutex>

class PacketSender : public QThread
{
    Q_OBJECT
public:
    PacketSender();

    //Accessors
    char *getBuffer() const;

    //Mutators
    void setBuffer(const char *buffer);
protected:
    void run();
private:
    char              *m_buffer;
    QMutex             m_mutex;
};

#endif // PACKETSENDER_H
