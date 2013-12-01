#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <list>
#include <QMainWindow>
#include <Types/camera.h>
#include "UI/videowindow.h"
#include "UI/icontrolcentermanager.h"
#include "Video/ffmpegwrapper.h"
#include <QThread>


namespace Ui {
class ControlCenter;
}

/**
 * @brief This class defines the control center for the video client.
 */
class ControlCenter : public QMainWindow, public IControlCenterManager
{
    Q_OBJECT
    
public:
    explicit ControlCenter(CameraList cameras, QWidget *parent = 0);
    ~ControlCenter();
    
private slots:
    void on_bOpen_clicked();

    //When a camera is closed
    void videoWindowClosed(Camera *cam);

    //Quit
    void quitActionClicked();

    void on_b_demux_test_clicked();

    /**
     * @brief Determine if the application is in learning mode or not.
     * @return True if in learning mode, false is not.
     */
    virtual bool inLearningMode();

private:
    Ui::ControlCenter *ui;
    Camera *c;

    //Functions
    void addCameras(CameraList cameras);

    //To test the demuxer
    FFMPEGWrapper           *m_ffmpeg;
    QThread                 *m_ffmpeg_thread;

};

#endif // CONTROLCENTER_H
