#ifndef TICKINGTIMER_H
#define TICKINGTIMER_H
#include <QTimer>
class TickingTimer : public QTimer
{
    Q_OBJECT
public:
    TickingTimer(QObject *parent = 0);
    void startTicking(int msecs);
    void startTicking(int msecs, int total_ticks);

    int getTicks() const;
    int getTickLimit() const;
private slots:
    //Tick slot
    void tick();
private:
    int             m_ticks;
    int             m_tick_limit;
};

#endif // TICKINGTIMER_H
