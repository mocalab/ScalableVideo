#ifndef FFMPEGVIDEOWIDGET_H
#define FFMPEGVIDEOWIDGET_H

#define PIPENAME    "/tmp/buffPipe1"

#include <QGLWidget>
#include <QThread>
#include <QTimer>
#include <QSharedPointer>
#include <Video/ffmpegwrapper.h>
#include <Video/demuxer.h>
#include <queue>

typedef std::queue<QImage *, std::list<QImage *> > FrameQueue;

class FFMPEGVideoWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit FFMPEGVideoWidget(QWidget *parent = 0);
    ~FFMPEGVideoWidget();
    void play();

public slots:
    //Get a new image
    void addImage(QImage *img);
    void timerTick();
signals:
    void resizeParent(int,int);
protected:
    void paintEvent(QPaintEvent *e);
private:
    //Thread and object for ffmpeg
    FFMPEGWrapper           *m_ffmpeg;
    QThread                 *m_ffmpeg_thread;
    Demuxer                 *m_demuxer;

    //Frame buffer
    FrameQueue              m_buffer;
    QImage                  *current_frame;

    bool                    first_frame;

    //A timer
    QTimer                  *m_timer;

    //Private functions


};

#endif // FFMPEGVIDEOWIDGET_H
