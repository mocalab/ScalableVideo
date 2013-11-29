#include "vlcvideowidget.h"
#define POSITION_RESOLUTION 10000
#include <QtGlobal>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QSlider>
#include <QTimer>
#include <QPushButton>
#include <QKeyEvent>
#include <vlc/libvlc.h>

#include "Global/Log.h"

VLCVideoWidget::VLCVideoWidget(QWidget *parent) :
    QWidget(parent)
{

//    //Set up sliders and timer
//    m_volumeSlider=new QSlider(Qt::Horizontal,this);
//    m_volumeSlider->setMaximum(100); //the volume is between 0 and 100
//    m_volumeSlider->setToolTip("Audio slider");

//    // Note: if you use streaming, there is no ability to use the position slider
//    m_positionSlider=new QSlider(Qt::Horizontal,this);
//    m_positionSlider->setMaximum(POSITION_RESOLUTION);

    m_poller = new QTimer(this);

    //Get a qframe for the video widget
    m_videoWidget = new QFrame(this);

    //Set up the box layout
    //QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget(m_videoWidget);
//    layout->addWidget(m_volumeSlider);
//    layout->addWidget(m_positionSlider);
    //setLayout(layout);

    //m_videoWidget->setStyleSheet("background-color:black;");
    m_videoWidget->resize(VIDEOWIDGET_DEFAULT_WIDTH, VIDEOWIDGET_DEFAULT_HEIGHT);
    //Signals and slots connections
//    connect(m_poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
//    connect(m_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
//    connect(m_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));
    m_vlc_interface = new VLCToQtInterface(m_videoWidget->width(), m_videoWidget->height());
    m_vlc_interface->initialize();
    m_poller->start(10);

    connect(m_vlc_interface, SIGNAL(resizeFrame(int,int)), this, SLOT(videoResized(int,int)));
}

VLCVideoWidget::~VLCVideoWidget()
{
    this->stopPlaying();
    delete m_videoWidget;
    //delete m_volumeSlider;
    m_poller->stop();
    delete m_poller;
    //delete m_positionSlider;
    delete m_vlc_interface;


}

void VLCVideoWidget::playFile(QString file)
{
    m_vlc_interface->setMediaToFile(file.toStdString().c_str());
    this->openMedia();
}

void VLCVideoWidget::playUrl(QString url)
{
    m_vlc_interface->setMediaToUrl(url.toStdString().c_str());
    this->openMedia();
}

//Stop
void VLCVideoWidget::stopPlaying()
{
    if(m_vlc_interface->isPlaying())
        m_vlc_interface->stop();
}

bool VLCVideoWidget::isPlaying()
{
    return this->m_vlc_interface->isPlaying();
}

void VLCVideoWidget::setFps(int fps)
{
    this->m_vlc_interface->setFps(fps);
}

int VLCVideoWidget::getFps()
{
    return m_vlc_interface->getFps();
}

void VLCVideoWidget::setPlaybackRate(int fps)
{
    this->m_vlc_interface->setPlaybackRate(fps);
}

void VLCVideoWidget::resetStats()
{
    this->m_vlc_interface->resetStats();
}

float VLCVideoWidget::getAverageBitrate()
{
    return this->m_vlc_interface->getCurrentBitrateAvg();
}
//Resize the video frame when this signal is called
void VLCVideoWidget::videoResized(int width, int height)
{
    m_videoWidget->resize(width, height);
    this->resize(width, height);
    //Signal to resize parent
    emit resizeParent(width, height);
}

//Overloaded paint event (to use stylesheet)
void VLCVideoWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//Private utility function to open the media file
void VLCVideoWidget::openMedia()
{
#ifdef Q_OS_LINUX
    int winId = (int)m_videoWidget->winId();
#elif defined(Q_OS_WIN32)
    void *winId = (void *)m_videoWidget->winId();
#endif
    m_vlc_interface->play(winId);

}

void VLCVideoWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Down)
    {
        this->stopPlaying();
    }
}

//SLOTS
//void VLCVideoWidget::changeVolume(int newVolume)
//{
//    libvlc_audio_set_volume(m_mp, newVolume);
//}
//void VLCVideoWidget::changePosition(int newPosition)
//{
//    //Make sure media player is playing something
//    libvlc_media_t *currMedia = libvlc_media_player_get_media(m_mp);
//    if(currMedia == NULL)
//        return;

//    float pos = (float)newPosition / (float)POSITION_RESOLUTION;
//    libvlc_media_player_set_position(m_mp, pos);
//}
//void VLCVideoWidget::updateInterface()
//{
//    if(!m_isPlaying)
//            return;

//   // It's possible that the vlc doesn't play anything
//   // so check before
//   libvlc_media_t *curMedia = libvlc_media_player_get_media (m_mp);

//   if (curMedia == NULL)
//       return;

//   float pos=libvlc_media_player_get_position (m_mp);
//   int siderPos=(int)(pos*(float)(POSITION_RESOLUTION));
//   m_positionSlider->setValue(siderPos);
//   int volume=libvlc_audio_get_volume (m_mp);
//   m_volumeSlider->setValue(volume);
//}
