#include "monitorthread.h"
#include "CollectedStatistics.h"
#include "reader_thread.h"

#include <QDebug>

MonitorThread::MonitorThread(QObject *parent) :
    QThread(parent)
{

    enabled = false;
    active = true;

    stats.packetsSent1 = 0;
    stats.packetsSent2 = 0;

    stats.packetsDropped1 = 0;
    stats.packetsDropped2 = 0;

    stats.bandwidth1 = 0;
    stats.bandwidth2 = 0;

    stats.bufferSize1 = 0;
    stats.bufferSize2 = 0;
}

void MonitorThread::stop()
{
    enabled = false;
   // if (!enabled )
  //      cond.wakeOne();
    qDebug() << "Timer stopped";
    wait();
}

//not used
void MonitorThread::pause()
{
    enabled = false;
    qDebug() << "Timer stopped";
}

void MonitorThread::pollValues(ReaderThread* thread1,ReaderThread* thread2,int freq)
{
    readerThread1 = thread1;
    readerThread2 = thread2;

    if ( !isRunning() )
        start();
 //   else if (!enabled )
 //       cond.wakeOne();

    enabled = true;
    active = true;
    frequency = freq;
}

void MonitorThread::run()
{
    counter = 0;
//    mutex.lock();
    while(enabled)
    {
      //  if (!enabled)
      //      cond.wait(&mutex);

        stats.bufferSize1 = readerThread1->getBufferSize();
        stats.bufferSize2 = readerThread2->getBufferSize();

        stats.packetsSent1 = readerThread1->getTotalPacketsSent();
        stats.packetsSent2 = readerThread2->getTotalPacketsSent();

        stats.packetsDropped1 = readerThread1->getTotalPacketsDropped();
        stats.packetsDropped2 = readerThread2->getTotalPacketsDropped();

        stats.bandwidth1 = readerThread1->getTotalBytesRead()*8 / 1000000.0;
        stats.bandwidth2 = readerThread2->getTotalBytesRead()*8 / 1000000.0;

        readerThread1->resetTotalBytesRead();
        readerThread2->resetTotalBytesRead();

        emit statisticsCollected(&stats);

        this->msleep(frequency);
    }
}
