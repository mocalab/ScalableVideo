#ifndef VLCTOQTINTERFACE_H
#define VLCTOQTINTERFACE_H
#include <QObject>
#include <Types/mediastatistics.h>
#include "vlcwrapper.h"

class QTimer;

class VLCToQtInterface : public QObject
{
     Q_OBJECT
public:
    /**
     * @brief VLCToQtInterface default value constructor.
     */
    VLCToQtInterface();
    /**
     * @brief VLCToQtInterface explicit value constructor.
     * @param width The initial video width.
     * @param height The initial video height.
     */
    VLCToQtInterface(int width, int height);
    /**
     * @brief Qt interface function to initialize the VLC client.
     */
    void initialize();
    /**
     * @brief ~VLCToQtInterface
     */
    virtual ~VLCToQtInterface();

    /**
     * @brief Qt interface function to release VLC allocated resources.
     */
    void freeResources();

    /**
     * @brief Interface to set media to a file.
     *Sets the VLC client's media object to play a certain file.
     * @param file_name The path to the file to play.
     */
    void setMediaToFile(const char *file_name);

    /**
     * @brief Interface to set the media to a URL.
     *Sets the VLC client's media object to stream from a specified URL.
     * @param url The URL of the media to stream.
     */
    void setMediaToUrl(const char *url);

    /**
     * @brief Play the media.
     *This interface function will inform the VLC client to begin playing, displaying the video in
     *using the passed window handle.
     * @param winId The handle to an XWindow window (Linux).
     * @return 0 on success, -1 on failure.
     */
    int play(uint32_t winId);

    /**
     * @brief Play the media.
     *This interface function will inform the VLC client to begin playing, displaying the video in
     *using the passed window handle.
     * @param winId The handle to an HWND window (Windows).
     * @return 0 on success, -1 on failure.
     */
    int play(void *winId);

    /**
     * @brief Inform the VLC client to stop playing.
     */
    void stop();

    /**
     * @brief Inform the VLC client to pause.
     */
    void pause();

    /**
     * @brief Sets the frame rate.
     *Set the frame rate of the video to be played.
     * @param fps The frame rate.
     */
    void setFps(int fps);

    /**
     * @brief Set the video playback rate.
     *Set the VLC client's video playback rate.
     * @param fps The playback rate.
     */
    void setPlaybackRate(int fps);

    //ACCESSORS
    /**
     * @brief Whether or not the VLC client is playing
     * @return true if playing, false otherwise
     */
    bool isPlaying();
    /**
     * @brief Retrieve the video's frame rate.
     * @return The frame rate of the video.
     */
    int getFps();
    /**
     * @brief Retrieve the media statistics.
     *Function to retrieve the statistics of the current media that is playing (i.e video bitrate).
     * @param statistics MediaStatistics object to store the statistics in.
     * @return true if the statistics are retrievable and the video is playing, false otherwise.
     */
    bool getMediaStats(MediaStatistics& statistics);

    /**
     * @brief Interface function to get the average bitrate for the current segment of video.
     * @return The average bitrate.
     */
    float getCurrentBitrateAvg();

    /**
     * @brief Reset the running bitrate average.
     *Interface function to reset the running average of the video bitrate (as in when a new segment of video is started).
     */
    void resetStats();

public slots:
    /**
     * @brief Update video size slot.
     *Slot that is called from a timer to determine if the parent widget needs to resize the
     *video container.
     */
    void updateVideoSize();

signals:
    /**
     * @brief Signal to resize.
     *Signal that is emitted to inform the containing widget to resize the video frame.
     * @param width The new frame width.
     * @param height The new frame height.
     */
    void resizeFrame(int width, int height);
//private members
private:
    //Functions
    /**
     * @brief Initialize the timer.
     */
    void initTimer();

    //Data members
    int             m_width;
    int             m_height;
    VLCWrapper      m_vlc;

    QTimer          *m_timer;
};

#endif // VLCTOQTINTERFACE_H
