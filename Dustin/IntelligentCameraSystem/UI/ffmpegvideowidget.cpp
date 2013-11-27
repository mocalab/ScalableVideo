#include "ffmpegvideowidget.h"
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
FFMPEGVideoWidget::FFMPEGVideoWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    first_frame(true)
{
    setFixedSize(320, 240);
    setAutoFillBackground(false);
    //Set up the demuxer
    m_demuxer = new Demuxer(QString(PIPENAME));
    m_ffmpeg = new FFMPEGWrapper(39082, m_demuxer);
    m_ffmpeg_thread = new QThread(this);
    m_ffmpeg->moveToThread(m_ffmpeg_thread);
    connect(m_ffmpeg_thread, SIGNAL(started()), m_ffmpeg, SLOT(demux()));
    connect(m_ffmpeg, SIGNAL(finished()), m_ffmpeg_thread, SLOT(quit()));

    connect(m_demuxer, SIGNAL(newImage(QImage *)), this, SLOT(addImage(QImage *)));

    //Set up the timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerTick()));

}

void FFMPEGVideoWidget::play()
{
    m_ffmpeg_thread->start();
    m_timer->start(1000/30);
}

FFMPEGVideoWidget::~FFMPEGVideoWidget()
{
}

void FFMPEGVideoWidget::addImage(QImage *img)
{
    m_buffer.push(img);
}

void FFMPEGVideoWidget::paintEvent(QPaintEvent *e)
{
    if(!first_frame)
    {

        QPainter p(this);

        //Set the painter to use a smooth scaling algorithm.
       p.setRenderHint(QPainter::SmoothPixmapTransform);

        p.begin(this);
        p.drawImage(e->rect(), *current_frame);
        p.end();


    }
}



void FFMPEGVideoWidget::timerTick()
{

    //Delete first frame
    if(!first_frame)
    {
        if(!m_buffer.empty()) ;
            delete current_frame;
    }

    //Get the next frame

    if(!m_buffer.empty())
    {
        if(first_frame)
            first_frame = false;
        current_frame = m_buffer.front();
        m_buffer.pop();

        repaint();
    }
}

