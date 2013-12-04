#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include "CollectedStatistics.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class ReaderThread;

class MonitorThread : public QThread
{
    Q_OBJECT
public:
    explicit MonitorThread(QObject *parent = 0);
    void pause();
    void stop();
    void pollValues(ReaderThread* thread1,ReaderThread* thread2,int freq);
signals:
    void statisticsCollected(CollectedStatistics* stats);

public slots:

private slots:

private:
    void run();

   // QTimer monitorTimer;
    int frequency;
    int counter;
    bool enabled;
    bool active;

    QMutex mutex;
    QWaitCondition cond;
    CollectedStatistics stats;
    ReaderThread *readerThread1;
    ReaderThread *readerThread2;

};

#endif // MONITORTHREAD_H
