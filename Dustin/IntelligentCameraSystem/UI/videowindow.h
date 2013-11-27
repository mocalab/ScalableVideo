#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#define PLAY_WITH_VLC 01
#define CONNECTION_MULTIPLE_ATTEMPTS 0

#include <QDialog>
#include <Types/camera.h>
#include <Network/networktoqtinterface.h>
#include <QThread>
#include <Util/FileUtil/bandwidthfilereader.h>

#define VIDEO_URL "udp/h264://@:39082"

namespace Ui {
class VideoWindow;
}

/**
 * @brief The VideoWindow class.
 *A window that contains the media player and media controls for a particular camera or stremaing server.
 */
class VideoWindow : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief Explicit value constructor.
     * @param camera The camera or streaming server whose video this window will play.
     * @param parent The parent widget of the window.
     */
    explicit VideoWindow(Camera *camera, QWidget *parent = 0);
    ~VideoWindow();
    //FOR TESTING PURPOSES
    void streamVideo(QString url);
    //bool eventFilter(QObject *obj, QEvent *event);


    /**
     * @brief Send a request.
     *Sends a request to the server and waits for a response.
     * @param data The data to send in the request.
     */
    void sendRequest(QString data);

    //Camera getter/setter
    /**
     * @brief Getter for the camera object.
     * @return The camera this video window streams from.
     */
    Camera *camera() const;
    /**
     * @brief The camera setter.
     * @param camera The camera to stream from.
     */
    void setCamera(Camera *camera);

    /**
     * @brief Overloaded equivalence operator.
     *Tests to see if two video windows are streaming from the same camera.
     * @param rhs The camera to test against.
     * @return True if the two cameras are the same, false otherwise.
     */
    bool operator==(const VideoWindow& rhs);

protected:
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);
public slots:
    void resizeWindow(int width, int height);
    void receivedMessage(QString response);
    void delayServerConnect();
    void disconnect();
    void sendButtonClicked();
    //Slot to send message to server to resize the video
    void resizeVideo(QString width, QString height, QString fps, QString bps);

    /**
     * @brief A slot invoked when the bandwidth has changed.
     * @param bandwidth The new bandwidth.
     */
    void onBandwidth(QString bandwidth);
signals:
    void sendCamera(Camera *);

private:
    Ui::VideoWindow *ui;
    //TCP connection
    NetworkToQtInterface    *m_tcp_interface;
    //TCP request/response thread
    QThread                 *m_request_thread;
    //Server address
    //Camera associated with this window
    Camera              *m_camera;

    //A file reader to get new bandwidths
    BandwidthFileReader         *m_bwfilereader;
    //A thread to run the reader loop in
    QThread                     *m_bwreaderthread;

    //The current bandwidth of the video
    int                         m_currentbw_kbps;

    bool                m_controls_revealed;
    bool                m_is_inside;

    //Functions
    //Connect to the camera server
    void connectToServer();
    void setUpThreads();

};

#endif // VIDEOWINDOW_H
