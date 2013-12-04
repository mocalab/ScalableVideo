#ifndef NETWORKINTERFACEWRITERTHREAD_H
#define NETWORKINTERFACEWRITERTHREAD_H

#define HAVE_REMOTE

#include <QThread>
#include <QMutex>
#include <pcap.h>
#include <remote-ext.h>

class Buffer;

class ReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit ReaderThread(QObject *parent = 0);

    void readFromAdapter(int interfaceNumber_,
                         pcap_t* pAdapter,
                         Buffer* buffer_);

    void stop();

    unsigned long getTotalBytesRead();
    void resetTotalBytesRead();

    unsigned long getTotalPacketsSent();
    unsigned long getTotalPacketsDropped();
    unsigned long getBufferSize();

    void setLossRate(int lossrate);
signals:

public slots:

private:
    void run();
    bool isThisPacketToBeDropped();
    void increaseTotalBytesRead(int value);

    int packetCount;


    unsigned long totalBytes;
    unsigned long packetsSent;
    unsigned long packetsDropped;

    QMutex mutexLossRate;
    QMutex mutexBytesRead;

    int interfaceNumber;
    pcap_t* pAdapter;
    float lossrate;
    bool quit;
    Buffer* buffer;
};

#endif // NETWORKINTERFACEWRITERTHREAD_H
