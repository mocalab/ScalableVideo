/**
 *  @file Definition of the video widget which serves as the user interface to the media player.
 */
#ifndef VLCVIDEOWIDGET_H
#define VLCVIDEOWIDGET_H

#define VIDEOWIDGET_DEFAULT_WIDTH 32
#define VIDEOWIDGET_DEFAULT_HEIGHT 40

#include <QWidget>
#include <vlc/vlc.h>
#include <Video/vlctoqtinterface.h>
#include <QThread>
//Forward class declarations
class QFrame;
class QVBoxLayout;
class QSlider;
class QTimer;
class QPushButton;
//class VLCToQtInterface;

/**
 * @brief This class defines a widget that uses a VLC client to display video.
 */
class VLCVideoWidget : public QWidget
{
private:
    Q_OBJECT
    //Volume and position sliders
    //QSlider                 *m_positionSlider;
    //QSlider                 *m_volumeSlider;
    //Timer
    QTimer                  *m_poller;
#ifdef Q_WS_X11
    QX11EmbedContainer      *m_videoWidget;
#else
    QFrame                  *m_videoWidget;
#endif

    VLCToQtInterface         *m_vlc_interface;

    //Utility methods
    void openMedia();

protected:
    void keyPressEvent(QKeyEvent *e);
public:
    /**
     * @brief Explicit value constructor.
     * @param parent The parent widget.
     */
    explicit VLCVideoWidget(QWidget *parent = 0);

    ~VLCVideoWidget();

    /**
     * @brief Play a video from a file on this widget.
     * @param file The path of the file to play.
     */
    void playFile(QString file);
    /**
     * @brief Stream the video at this URL.
     * @param url The URL to stream video from.
     */
    void playUrl(QString url);

    /**
     * @brief Stop playing the video.
     */
    void stopPlaying();
    /**
     * @brief Determine if the video is playing.
     * @return True if the video is playing.
     */
    bool isPlaying();
    /**
     * @brief Set the frame rate of the playing video.
     * @param fps The frame rate of the video.
     */
    void setFps(int fps);
    /**
     * @brief Get the frame rate of the video.
     * @return The video frame rate.
     */
    int getFps();
    /**
     * @brief Set the VLC client playback rate.
     * @param fps The playback rate.
     */
    void setPlaybackRate(int fps);

    /**
     * @brief Reset the values of the statistics captured from the VLC client for the current video.
     */
    void resetStats();

    /**
     * @brief Get the current running average of the input bitrate.
     * @return The averate data rate.
     */
    float getAverageBitrate();

    /**
     * @brief Reset VLC client's media options.
     */
    void setMediaOptions();

    /**
     * @brief Take a snapshot of the video currently being played.
     * @param filepath The file into which the screenshot should be saved.
     * @return True if successful, false otherwise.
     */
    bool takeSnapshot(QString &filepath);

signals:
    void resizeParent(int width, int height);
public slots:
//    void updateInterface();
//    void changeVolume(int newVolume);
//    void changePosition(int newPosition);
    /**
     * @brief A slot invoked when the size of the video changes.
     * @param width The new width of the video.
     * @param height The new height of the video.
     */
    void videoResized(int width, int height);
protected:
    void paintEvent(QPaintEvent *e);
    
};

#endif // VLCVIDEOWIDGET_H
