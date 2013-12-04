#include "writer_thread.h"
#include "buffer.h"
#include "PerformanceTimers.h"

#include <QDebug>

WriterThread::WriterThread(QObject *parent) :
    QThread (parent)
{
    interfaceNumber = 0;
    bandWidthLimit = 0;
    pAdapter = NULL;
    quit = true;
}

void WriterThread::setBandwidthLimit(double bandWidthLimit_)
{
    mutex.lock();
    bandWidthLimit = bandWidthLimit_;
    mutex.unlock();
}

void WriterThread::stop()
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }
}

void WriterThread::writeToAdapter(int interfaceNumber_,
                                  pcap_t* pAdapter_,
                                  Buffer* buffer_ )
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

    start();
}

void WriterThread::run()
{
    qDebug()  << "Thread ID: " <<  QThread::currentThreadId() << " Starting to write traffic on interface " << interfaceNumber;
    const u_char *pkt_data;
    int size;
    int early;
    int totalBytes = 0;
    unsigned int bandConsumed = 0;

    pTimer loopTimer;

    InitializeTimers();
    loopTimer = StartTimer();
    early = GetMicrosecondsElapsed(loopTimer);

    unsigned int limit = 0;

    while(!quit)
    {
        if ( bandWidthLimit == 0)
            this->msleep(1000);
        else
            limit = bandWidthLimit * 1000000;

        if (GetMicrosecondsElapsed(loopTimer)-early <  ONE_MILLION_USEC  )
        {
            if ( bandConsumed < limit )
            {
                size = buffer->removePacket(&pkt_data);

                if ( size == 0 )
                {
                    quit = true;
                    break;
                }

                pcap_sendpacket(pAdapter, pkt_data, size);

                totalBytes += size;
                bandConsumed = totalBytes * 8;
            }
        }
        else
        {
            early = GetMicrosecondsElapsed(loopTimer);
            totalBytes = 0;
            bandConsumed = 0;
        }
    }
    qDebug() << "Writter thread "<< interfaceNumber <<  " exited";
}
