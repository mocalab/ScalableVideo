#ifndef TRAFFICBRIDGE_H
#define TRAFFICBRIDGE_H

#define HAVE_REMOTE
#define PACKET_CAPTURE_SIZE 65536

#include "reader_thread.h"
#include "writer_thread.h"
#include "monitorthread.h"

#include <QObject>
#include <pcap.h>
#include <remote-ext.h>

class Buffer;
struct CollectedStatistics;

struct InterfaceData
{
    QString hardwareAddress;
    QString interfaceName;
};

class NetworkEmulator : public QObject
{
    Q_OBJECT
public:
    explicit NetworkEmulator(QObject *parent = 0);
    ~NetworkEmulator();

    void lookupAdapterInterfaces();
    void setSelectedInterfaces( int interface1, int interface2);
    void setMacFilters(QString filterInterfaceOne, QString filterInterfaceTwo );
    void startBridge();
    void setLossrate(int lossRate1,int lossRate2);
    void setBandwidth(double bandwidth1,double bandwidth2);
    void Stop();
signals:
    void addAdapterInterface(QString interfaceDescription);
    void statisticsCollected(CollectedStatistics* stats);
public slots:
    void receiveStatistics(CollectedStatistics* stats);

private:
    QString getInterfaceHardwareAddress(QString name);

    QList<InterfaceData> interfaceDataList;
    int selectedInterfaceIndex1;
    int selectedInterfaceIndex2;

    void receiveTraffic();
    int setupWinPcapFilter(pcap_t** fp, QString macFilter);

    MonitorThread monitorThread;

    WriterThread writerThread1;
    WriterThread writerThread2;

    ReaderThread readerThread1;
    ReaderThread readerThread2;

    pcap_t* pAdapterOne;
    pcap_t* pAdapterTwo;

    Buffer* buffer1;
    Buffer* buffer2;
};

#endif // TRAFFICBRIDGE_H
