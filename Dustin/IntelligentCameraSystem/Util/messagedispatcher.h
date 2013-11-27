#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <QObject>
#include <string>

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    MessageDispatcher();
    void reemitMessage(const char * msg);
signals:
    void passMessage(const char *);
};

#endif // MESSAGEDISPATCHER_H
