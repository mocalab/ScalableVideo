#ifndef NETWORKINTERFACETHREAD_H
#define NETWORKINTERFACETHREAD_H

#define HAVE_REMOTE
#define ONE_MILLION_USEC 1000000

#include <QThread>
#include <QMutex>
#include <pcap.h>
#include <remote-ext.h>

class Buffer;

class WriterThread : public QThread
{
    Q_OBJECT
public:
    explicit WriterThread(QObject *parent = 0);

    void setBandwidthLimit(double bandwidth);

    void writeToAdapter(int interfaceNumber_,
                        pcap_t* pAdapter,
                        Buffer* buffer_);

    void stop();

signals:

public slots:

private:
    void run();

    int interfaceNumber;
    double bandWidthLimit;
    pcap_t* pAdapter;
    QMutex mutex;
    bool quit;
    Buffer* buffer;
};

#endif // NETWORKINTERFACETHREAD_H

// qDebug() << "UN-Locking by thread " << QThread::currentThreadId();
//bufferEmpty.wakeAll();
