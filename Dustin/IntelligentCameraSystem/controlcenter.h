#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <list>
#include <QMainWindow>
#include <Types/camera.h>
#include "UI/videowindow.h"
#include "Video/ffmpegwrapper.h"
#include <QThread>

typedef list<Camera *>                  CameraList;
typedef list<Camera *>::iterator        CameraListIterator;

namespace Ui {
class ControlCenter;
}

class ControlCenter : public QMainWindow
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
