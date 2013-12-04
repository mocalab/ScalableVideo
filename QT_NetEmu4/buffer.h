#ifndef BUFFER_H
#define BUFFER_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <windows.h>

class Buffer
{
public:
    Buffer(long size);
    void addPacket(const uchar* packet, int size);
    int removePacket(const uchar** packet);
    int getSize();
    void reset();

private:
    QQueue<const u_char*> FIFO;
    QQueue<int> sizeFIFO;
    QMutex mutexFIFO;

    long bufferSize;

    bool quit;

    QWaitCondition bufferEmpty;
    QMutex wMutex;

};

#endif // BUFFER_H
