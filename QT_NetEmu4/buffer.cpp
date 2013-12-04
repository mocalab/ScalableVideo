#include "buffer.h"
#include "QDebug"

#include <QThread>

Buffer::Buffer(long size=1000)
{
}

void Buffer::addPacket(const uchar* packet, int size)
{
    mutexFIFO.lock();
    FIFO.enqueue(packet);
    sizeFIFO.enqueue(size);
    mutexFIFO.unlock();

    wMutex.lock();
    bufferEmpty.wakeOne();
    wMutex.unlock();
}

int Buffer::removePacket(const uchar** packet)
{
    while ( sizeFIFO.size() == 0)
    {
        wMutex.lock();
        bufferEmpty.wait(&wMutex);
        wMutex.unlock();

        // only occures if reset is called
        if ( sizeFIFO.size() == 0 )
            return 0;

    }

    mutexFIFO.lock();
    *packet = FIFO.dequeue();
    int size = sizeFIFO.dequeue();
    mutexFIFO.unlock();

    return size;
}

int Buffer::getSize()
{
    return sizeFIFO.size();
}

void Buffer::reset()
{
    mutexFIFO.lock();
    FIFO.clear();
    sizeFIFO.clear();
    mutexFIFO.unlock();

    wMutex.lock();
    bufferEmpty.wakeOne();
    wMutex.unlock();
}
