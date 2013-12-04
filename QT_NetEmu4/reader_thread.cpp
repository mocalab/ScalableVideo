#include "reader_thread.h"
#include "buffer.h"
#include "PerformanceTimers.h"

#include <cmath>
#include <QDebug>

ReaderThread::ReaderThread(QObject *parent) :
    QThread(parent)
{
    interfaceNumber = 0;
    lossrate = 0;
    packetCount = 0;
    pAdapter = NULL;
    quit = true;
    packetsSent = 0;
    packetsDropped = 0;

}

void ReaderThread::stop()
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }
}

void ReaderThread::setLossRate(int lossrate_)
{
    mutexLossRate.lock();
    lossrate = lossrate_;
    mutexLossRate.unlock();
}

void ReaderThread::readFromAdapter(int interfaceNumber_,
                                   pcap_t* pAdapter_,
                                   Buffer* buffer_)
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }

    interfaceNumber = interfaceNumber_;
    pAdapter = pAdapter_;
    buffer = buffer_;
    quit = false;
    resetTotalBytesRead();
    packetsSent = 0;
    packetsDropped = 0;

    start();
}

unsigned long ReaderThread::getTotalPacketsSent()
{
    return packetsSent;
}

unsigned long ReaderThread::getTotalPacketsDropped()
{
    return packetsDropped;
}

unsigned long ReaderThread::getBufferSize()
{
    return buffer->getSize();
}

unsigned long ReaderThread::getTotalBytesRead()
{
    mutexBytesRead.lock();
    unsigned long temp = totalBytes;
    mutexBytesRead.unlock();
    return temp;
}

void ReaderThread::resetTotalBytesRead()
{
    mutexBytesRead.lock();
    totalBytes = 0;
    mutexBytesRead.unlock();
}

void ReaderThread::increaseTotalBytesRead(int value)
{
    mutexBytesRead.lock();
    totalBytes += value;
    mutexBytesRead.unlock();
}

bool ReaderThread::isThisPacketToBeDropped()
{
    mutexLossRate.lock();
    int currentLossRate = lossrate;
    mutexLossRate.unlock();

    bool result;

    if ( currentLossRate == 0)
        result = false;
    {
        double packetsToSend = 100.0/currentLossRate;

        if ( currentLossRate > 50 )
            packetsToSend = ceil(1/(packetsToSend-1));

        if ( currentLossRate > 50)
        {
            if ( packetCount < packetsToSend )
                result = true;
            else
            {
                packetCount = 0;
                result = false;
            }
        }
        else
        {
            if ( packetCount < packetsToSend )
                result = false;
            else
            {
                packetCount = 0;
                result = true;
            }
        }
    }

    packetCount++;

    if ( result )
    {
        packetsDropped++;
        //qDebug() << "Packet dropped from interface " << interfaceNumber ;
    }
    else
    {
        packetsSent++;

       // qDebug() << "Packet sent from interface " << interfaceNumber ;
    }

    return result;
}

void ReaderThread::run()
{
    qDebug()  << "Thread ID: " <<  QThread::currentThreadId() << " Starting to receive traffic on interface " << interfaceNumber;
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    int res;

    while(!quit && (res = pcap_next_ex( pAdapter , &header, &pkt_data)) >= 0)
    {
        if(res == 0)
            continue; // Timeout elapsed

        increaseTotalBytesRead( header->caplen );

        if (  !isThisPacketToBeDropped() )
            buffer->addPacket(pkt_data,header->caplen);
    }
    qDebug() << "Reader thread "<< interfaceNumber <<  " exited";
}


