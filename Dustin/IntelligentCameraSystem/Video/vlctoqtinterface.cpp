#include <QTimer>
#include "vlctoqtinterface.h"

VLCToQtInterface::VLCToQtInterface() :
    m_width(320),
    m_height(240)
{
    this->initTimer();
}

VLCToQtInterface::VLCToQtInterface(int width, int height) :
    m_width(width),
    m_height(height)
{
    this->initTimer();
}

//Initialize the timer
void VLCToQtInterface::initTimer()
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateVideoSize()));
    //Timeout every 500 ms
    m_timer->start(500);

}

//Initialize an instance of VLC
void VLCToQtInterface::initialize()
{
    m_vlc.initialize();
}

VLCToQtInterface::~VLCToQtInterface()
{
    m_timer->stop();
    delete m_timer;
}

//Tear down
void VLCToQtInterface::freeResources()
{
    m_vlc.freeResources();
}

//Open from file
void VLCToQtInterface::setMediaToFile(const char *file_name)
{
    //Open the file specified in the open string
    m_vlc.setMediaToFile(file_name);
}

//Open from URL
void VLCToQtInterface::setMediaToUrl(const char *url)
{
    //Open specified url
    m_vlc.setMediaToFile(url);
}

//Play the media
int VLCToQtInterface::play(uint32_t winId)
{
    int status = 0;

    status = m_vlc.play(winId);

    return status;
}

int VLCToQtInterface::play(void *winId)
{
    int status = 0;
    status = m_vlc.play(winId);
    return status;
}

void VLCToQtInterface::stop()
{
    m_vlc.stop();
}

void VLCToQtInterface::pause()
{
    m_vlc.pause();
}

void VLCToQtInterface::setFps(int fps)
{
    this->m_vlc.setFps(fps);
}

void VLCToQtInterface::setPlaybackRate(int fps)
{
    this->m_vlc.resetMedia(fps);
}


bool VLCToQtInterface::isPlaying()
{
    return m_vlc.isPlaying();
}

int VLCToQtInterface::getFps()
{
    return this->m_vlc.getFps();
}

//Get the current playing media statistics
bool VLCToQtInterface::getMediaStats(MediaStatistics& statistics)
{
    //Get the important media statistics to be used by the caller.
    bool err = true;
    libvlc_media_stats_t stats;
    if((err = m_vlc.getMediaStats(&stats)))
    {
        //Place in a MediaStatistics object
        statistics.setDecodedVideo(stats.i_decoded_video);
        statistics.setDemuxBitrate(stats.f_demux_bitrate);
        statistics.setDemuxCorrupted(stats.i_demux_corrupted);
        statistics.setDemuxDiscontinuity(stats.i_demux_discontinuity);
        statistics.setDemuxReadBytes(stats.i_demux_read_bytes);
        statistics.setDisplayedPictures(stats.i_displayed_pictures);
        statistics.setInputBitrate(stats.f_input_bitrate);
        statistics.setLostPictures(stats.i_lost_pictures);
        statistics.setPlayedPictures(stats.i_played_abuffers);
        statistics.setReadBytes(stats.i_read_bytes);

    }

    return err;
}

//Get the average bitrate for this segment of video
float VLCToQtInterface::getCurrentBitrateAvg()
{
    return m_vlc.getCurrentBitrateAvg();
}

//Reset the running average of the bitrate
void VLCToQtInterface::resetStats()
{
    m_vlc.resetStats();
}

void VLCToQtInterface::setMediaOptions()
{
    m_vlc.pause();
    m_vlc.setMediaOptions();
    m_vlc.resume();

}

//Take a screenshot
bool VLCToQtInterface::takeSnapshot(QString &filepath)
{
    return m_vlc.takeSnapshot(filepath.toStdString().c_str());
}

//---------------------SLOTS---------------------------------
//Check to update frame size, called from timer
void VLCToQtInterface::updateVideoSize()
{
    int width = 0, height = 0;
    //Get width and height, store in local variabled, return if video is playing
    bool playing = m_vlc.getVideoSize(&width, &height);
    //If video is playing
    if(playing)
    {
        if(width == 0 && height == 0)
        {
            //Set new widths and heights and emit resize signal
            //this->m_width = 320;
            //this->m_height = 240;
            emit resizeFrame(m_width, m_height);
        }
        //If width and height are different
        else if((width) != this->m_width || (height) != this->m_height)
        {
            //Set new widths and heights and emit resize signal
            this->m_width = width;
            this->m_height = height;
            emit resizeFrame(width, height);
        }

    }
    else
    {
        if((width) != 320 && (height) != 240)
        {
            this->m_width = 320;
            this->m_height = 240;
            emit resizeFrame(m_width, m_height);
        }

    }

    //Get media stats
    if(m_vlc.isPlaying())
        this->m_vlc.getMediaStats();
}
