#include "networkinterfacethread.h"
#include "buffer.h"

#include <QDebug>

NetworkInterfaceReaderThread::NetworkInterfaceReaderThread(QObject *parent) :
    QThread (parent)
{
    interfaceNumber = 0;
    lossrate = 0;
    pAdapter_Read = NULL;
    pAdapter_Write = NULL;
    quit = true;
}

void NetworkInterfaceReaderThread::setLossRate(float lossrate_)
{
    mutex.lock();
    lossrate = lossrate_;
    mutex.unlock();
}

void NetworkInterfaceReaderThread::stop()
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }
}

void NetworkInterfaceReaderThread::receiveTraffic(int interfaceNumber_, pcap_t* pAdapter_Read_, pcap_t* pAdapter_Write_ )
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }

    interfaceNumber = interfaceNumber_;
    pAdapter_Read = pAdapter_Read_;
    pAdapter_Write = pAdapter_Write_;
    quit = false;
    start();
}

void NetworkInterfaceReaderThread::run()
{
    qDebug() <<"Starting to receive traffic on interface " << interfaceNumber;

    /*          */

    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    int   istobelost;
    int res;
    float randomnum;
    float currentLossRate;
    int   lossnum;
    int k = 0;
    int l = 0;

    srand ( time(NULL) );

    while(!quit && (res = pcap_next_ex( pAdapter_Read , &header, &pkt_data)) >= 0)
    {
        if(res == 0)
            continue; // Timeout elapsed

        mutex.lock();
        currentLossRate = lossrate;
        mutex.unlock();

        if (currentLossRate > 0)
        {
            randomnum = rand()/(float) RAND_MAX;
            lossnum = 100 / currentLossRate;
            istobelost = (int) ((float) randomnum *lossnum );
        }
        else
            istobelost = 100;

        k++;
        if (currentLossRate != 0 )
            l++;

        if ( l < lossnum )
        {
            int status = pcap_sendpacket(pAdapter_Write, pkt_data, header->caplen);

        }
        else
           qDebug() << "packet dropped from interface " << interfaceNumber;
    }
}
