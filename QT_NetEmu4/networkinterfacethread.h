#ifndef NETWORKINTERFACETHREAD_H
#define NETWORKINTERFACETHREAD_H

#define HAVE_REMOTE

#include <QThread>
#include <QMutex>
#include <pcap.h>
#include <remote-ext.h>

class Buffer;

class NetworkInterfaceReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit NetworkInterfaceReaderThread(QObject *parent = 0);

    void setLossRate(float newLossRate);

    void receiveTraffic(int interfaceNumber_,
               pcap_t* pAdapter_Read_,
               pcap_t* pAdapter_Write_);

    void stop();

signals:

public slots:

private:
    void run();

    int interfaceNumber;
    float lossrate;

    pcap_t* pAdapter_Read;
    pcap_t* pAdapter_Write;
    QMutex mutex;
    bool quit;

    Buffer* buffer;
};

#endif // NETWORKINTERFACETHREAD_H
