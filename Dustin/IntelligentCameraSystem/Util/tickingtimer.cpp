#include "tickingtimer.h"

TickingTimer::TickingTimer(QObject *parent) :
    QTimer(parent),
    m_tick_limit(0)
{
    connect(this, SIGNAL(timeout()), this, SLOT(tick()));
}

void TickingTimer::startTicking(int msecs)
{
    m_ticks = 0;
    this->start(msecs);
}

void TickingTimer::startTicking(int msecs, int total_ticks)
{
    m_ticks = 0;
    m_tick_limit = total_ticks;
    this->start(msecs);
}

int TickingTimer::getTicks() const
{
    return m_ticks;
}
int TickingTimer::getTickLimit() const
{
    return m_tick_limit;
}

void TickingTimer::tick()
{
    m_ticks++;
    //If there is a tick ceiling, check to see if we should stop
    if(m_tick_limit != 0)
    {
        if(m_ticks > m_tick_limit)
        {
            //Stop if we have exceeded the limit
            this->stop();
        }
    }
}
