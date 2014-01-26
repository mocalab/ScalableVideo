#ifndef VLCWRAPPER_H
#define VLCWRAPPER_H
#include <vlc/vlc.h>
#include <QMutex>

class VLCWrapper
{
public:
    VLCWrapper();
    /**
     * @brief Initialize the VLC client.
     *This function will initialize the VLC instance and media player.
     */
    void initialize();
    //Destructor
    ~VLCWrapper();

    /**
     * @brief Delete all VLC resources.
     *Deletes any resources allocated by the VLC client.
     */
    void freeResources();

    /**
     * @brief Set the media object.
     *Sets the media object to the file whose full name is file_name.
     * @param file_name The name of the file to play.
     */
    void setMediaToFile(const char *file_name);

    /**
     * @brief Set the media object.
     *Set the media object to stream from the URL specified.
     * @param url The URL to stream from.
     */
    void setMediaToUrl(const char *url);

    /**
     * @brief Play the media.
     *Play the media and display the video on the widget using a handle to an XWindow. Used on Linux machines.
     * @param winId The handle to the XWindow.
     * @return 0 on success.
     */
    int play(uint32_t winId);

    /**
     * @brief Play the media.
     *Play the media and display the video on the widget using an HWND handle. Used on Windows machines.
     * @param winId The handle to the HWND.
     * @return 0 on success.
     */
    int play(void *winId);

    /**
     * @brief Resume video playback.
     */
    void resume();

    /**
     * @brief Stop playing.
     */
    void stop();

    /**
     * @brief Set the frame rate of the video to be played.
     * @param fps The frame rate.
     */
    void setFps(int fps);

    /**
     * @brief Pause function.
     */
    void pause();

    /**
     * @brief Get the current video size.
     *Get the size of the video playing and store the values in the integer pointers passed.
     * @param width The current video width.
     * @param height The current video height.
     * @return true if the video is playing, false otherwise.
     */
    bool getVideoSize(int *width, int *height);

    /**
     * @brief Reset the media.
     * @param fps The frame rate to set for playback.
     */
    void resetMedia(int fps);

    //ACCESSORS

    /**
     * @brief Return whether or not the video is playing.
     * @return true if playing, false otherwise.
     */
    bool isPlaying();
    /**
     * @brief Get the current video frame rate.
     * @return The current video frame rate.
     */
    int getFps();

    /**
     * @brief Retrieve the current media statistics.
     *Retrieves the current media statistics and prints their values to the log.
     *Obtain the current media statistics i.e. bitrate of the video.
     */
    void getMediaStats();

    /**
     * @brief Retrieve the current media statistics.
     *Retrieve the current media statistics and store them in the media stats object passed.
     * @param stats The media stats object to store the statistics in.
     * @return true if the stats are available and the media is playing, false otherwise
     */
    bool getMediaStats(libvlc_media_stats_t *stats);

    /**
     * @brief Get the average bitrate for the current segment of video.
     * @return The average bitrate.
     */
    float getCurrentBitrateAvg();

    /**
     * @brief Get the maximum bitrate for th current segment of video
     * @return The maximum bitrate;
     */
    float getCurrentMaxBitrate();

    /**
     * @brief Reset the running bitrate average.
     *This function will reset the running average of the video bitrate (as in when a new segment of video is started).
     */
    void resetStats();

    /**
     *  This function will set up the media options prior to playing.
     */
    void setMediaOptions();

    /**
     * @brief Take a snapshot of the video currently being played.
     * @param filepath The file into which the screenshot should be saved.
     * @return True if successful, false otherwise.
     */
    bool takeSnapshot(const char *filepath);

//private members
private:
    libvlc_instance_t       *m_vlc;
    libvlc_media_player_t   *m_mp;
    libvlc_media_t          *m_media;


    //If we are playing
    bool                     m_isPlaying;

    int                      m_fps;

    //A data pointer for the logging callback
    char                     m_data[1024];

    //Integer for keeping track of the average bitrate
    int                     m_current;
    //Average bitrate
    float                   m_bravg;

    //The max bitrate for the current segment
    float                   m_maxbitrate;

    //A mutex for the average shared resources
    QMutex                  m_avgmutex;

    //A mutex for the max bitrate
    QMutex                  m_maxmutex;

    //Private functions


};

extern "C"{
#include "stdio.h"
void log_msg(void *data, int level, const libvlc_log_t *ctx, const char *fmt, va_list args);
}

#endif // VLCWRAPPER_H
