#include "messagedispatcher.h"

MessageDispatcher::MessageDispatcher()
{
}

void MessageDispatcher::reemitMessage(const char * msg)
{
    emit passMessage(msg);
}
