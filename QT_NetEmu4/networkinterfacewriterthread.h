#ifndef NETWORKINTERFACEWRITERTHREAD_H
#define NETWORKINTERFACEWRITERTHREAD_H

#include <QObject>

class NetworkInterfaceWriterThread : public QObject
{
    Q_OBJECT
public:
    explicit NetworkInterfaceWriterThread(QObject *parent = 0);

signals:

public slots:

};

#endif // NETWORKINTERFACEWRITERTHREAD_H
