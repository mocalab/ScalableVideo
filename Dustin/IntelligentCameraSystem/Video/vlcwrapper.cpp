#include "vlcwrapper.h"
#include <string>
#include <vlc/libvlc.h>
#include <Global/Log.h>
VLCWrapper::VLCWrapper() :
    m_isPlaying(false),
    m_fps(30),
    m_bravg(0.0),
    m_current(0)
{
}

//Initialize an instance of VLC
void VLCWrapper::initialize()
{
    //Set up VLC command line parameters
    const char *vlc_args[] = {
        "-I", "dummy",
        "--ignore-config",
//        "--extraintf=logger",
//        "",
//        "--file-logging",
//        "--logfile=../logs/videostreamer-vlc.log"
        "--no-osd",
        "--clock-jitter=200",
        "--ffmpeg-threads=1"

    };

    //Instantiate vlc
    m_vlc = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    //m_vlc = libvlc_new(0,0);
    //Instantiate a media player
    m_mp = libvlc_media_player_new(m_vlc);


    //Logging callback -- HAVE TO UPGRADE UBUNTU TO GET LATEST VERSION OF LIBVLC
    //libvlc_log_set(m_vlc, log_msg, (void *)this->m_data);


}

VLCWrapper::~VLCWrapper()
{
    //call the teardown function
    this->freeResources();
}

//Tear down
void VLCWrapper::freeResources()
{
    libvlc_media_player_stop(m_mp);
    libvlc_media_player_release(m_mp);
    libvlc_release(m_vlc);
}

//Open from file
void VLCWrapper::setMediaToFile(const char *file_name)
{
    if(m_isPlaying)
    {
        //Stop current video
        this->stop();
    }
    //Open the file specified in the open string
    this->m_media = libvlc_media_new_path(m_vlc, file_name);
}

//Open from URL
void VLCWrapper::setMediaToUrl(const char *url)
{
    if(m_isPlaying)
    {
        //Stop current video
        this->stop();
    }
    //Open specified url
    this->m_media = libvlc_media_new_location(m_vlc, url);
}

//Play the media (Linux version)
int VLCWrapper::play(uint32_t winId)
{
    int status = 0;
    //Set media player element
    libvlc_media_player_set_media(m_mp, m_media);
    //Non-windows
    libvlc_media_player_set_xwindow(m_mp, winId);

    //Set the media options we desire
    this->setMediaOptions();

    //Play media
    status = libvlc_media_player_play(m_mp);

    if(status != -1)
        this->m_isPlaying = true;

    return status;
}

//Windows version
int VLCWrapper::play(void *winId)
{
    int status = 0;
    //Set media player element
    libvlc_media_player_set_media(m_mp, m_media);
    //Windows specific
    libvlc_media_player_set_hwnd(m_mp, winId);

    //Set the media options we desire
    this->setMediaOptions();

    //Play media
    status = libvlc_media_player_play(m_mp);

    if(status != -1)
        this->m_isPlaying = true;

    return status;
}

void VLCWrapper::resume()
{
    if(!this->m_isPlaying)
    {
        libvlc_media_player_set_media(m_mp, m_media);
        libvlc_media_player_play(m_mp);
        this->m_isPlaying = true;
    }
}

void VLCWrapper::stop()
{
    libvlc_media_player_stop(m_mp);
    //Release the media
    libvlc_media_release(m_media);
    this->m_isPlaying = false;
}

void VLCWrapper::setFps(int fps)
{
    this->m_fps = fps;
}

void VLCWrapper::pause()
{
    libvlc_media_player_pause(m_mp);
    this->m_isPlaying = false;
}

//Determine widht and height of video; returns false if video is not playing
bool VLCWrapper::getVideoSize(int *width, int *height)
{
    bool playing = this->m_isPlaying;
    //Only get width and height if video is playing
    if(playing)
    {
        //*width = libvlc_video_get_width(m_mp);
        //*height = libvlc_video_get_height(m_mp);
        libvlc_video_get_size(m_mp, 0, (unsigned int *)width, (unsigned int *)height);
    }
    return playing;
}

void VLCWrapper::resetMedia(int fps)
{
    //int ret = libvlc_media_player_set_rate(m_mp, ((float)fps) / 30);
    //Set the media options we desire
    this->setMediaOptions();

    //Set media player element
    libvlc_media_player_set_media(m_mp, m_media);

    //Play media
    int status = libvlc_media_player_play(m_mp);
}


bool VLCWrapper::isPlaying()
{
    return (1 == libvlc_media_player_is_playing(m_mp));
    //return this->m_isPlaying;
}

int VLCWrapper::getFps()
{
    return m_fps;
}

//Get the current media statistics
void VLCWrapper::getMediaStats()
{
    libvlc_media_stats_t stats;

    int err = libvlc_media_get_stats(m_media, &stats);

    if(err)
    {
        //Compute the running average

        //Lock the mutex
        m_avgmutex.lock();
        //Increment average counter
        m_current++;
        //Get the new average
        m_bravg = (m_bravg * (m_current-1) + stats.f_input_bitrate) / m_current;
        //Unlock the mutex
        m_avgmutex.unlock();

        //Get the maximum
        m_maxmutex.lock();

        if(m_maxbitrate == 0.0 || stats.f_input_bitrate > m_maxbitrate)
            m_maxbitrate = stats.f_input_bitrate;

        m_maxmutex.unlock();

        //Print
        //DEBUG() << stats.f_demux_bitrate << "    " << stats.f_input_bitrate << "    " << m_bravg << "    " << m_maxbitrate;

    }
    else
    {

        DEBUG() << "FAILED TO GET STATISTICS: " << err;
    }


}

//Get the media statistics and store them in a media stats object
bool VLCWrapper::getMediaStats(libvlc_media_stats_t *stats)
{
    bool status = true;
    //See if media is playing
    status = this->isPlaying();
    //Get the statistics
    if(status)
    {
       status = libvlc_media_get_stats(m_media, stats);
    }

    return status;
}

//Get the current average bitrate
float VLCWrapper::getCurrentBitrateAvg()
{
    float avg = 0.0;
    //Lock the mutex
    m_avgmutex.lock();
    avg = this->m_bravg;
    m_avgmutex.unlock();

    return avg;

}
//Get the max bitrate
float VLCWrapper::getCurrentMaxBitrate()
{
    float max = 0.0;
    //Lock the mutex
    m_maxmutex.lock();
    max = this->m_maxbitrate;
    m_maxmutex.unlock();

    return max;
}

//Reset the running average of the bitrate
void VLCWrapper::resetStats()
{
    //Lock the mutex
    m_avgmutex.lock();
    m_bravg = 0.0;
    m_current = 0;
    m_avgmutex.unlock();

    //Reset the maximum
    m_maxmutex.lock();
    m_maxbitrate = 0.0;
    m_maxmutex.unlock();

}



void VLCWrapper::setMediaOptions()
{
    std::string fps = std::string("h264-fps=") + (m_fps == 30 ? "40" : "20");

    libvlc_media_add_option(m_media, "network-caching=50");
    libvlc_media_add_option(m_media, fps.c_str());//"h264-fps=40");
    libvlc_media_add_option(m_media, "clock-jitter=200");
    libvlc_media_add_option(m_media, "auto-adjust-pts-delay");

    //libvlc_media_add_option(m_media, "no-osd");
}


//C Functions

void log_msg(void *data, int level, const libvlc_log_t *ctx, const char *fmt, va_list args)
{
    //Parse log message to see if it is a motion change; if so, get the number of motion areas
    //to determine motion content
    printf(fmt);
}
