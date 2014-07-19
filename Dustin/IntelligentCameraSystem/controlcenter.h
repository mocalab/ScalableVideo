#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <list>
#include <QMainWindow>
#include "Types/camera.h"
#include "UI/videowindow.h"
#include "UI/icontrolcentermanager.h"
#if !PLAY_WITH_VLC
#include "Video/ffmpegwrapper.h"
#endif
#include "LearningModule/learninginterface.h"
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
    explicit ControlCenter(CameraList cameras, bool standard_videos, QWidget *parent = 0);
    ~ControlCenter();
    /**
     * @brief Determine if the application is in learning mode or not.
     * @return True if in learning mode, false is not.
     */
    virtual bool inLearningMode();

    /**
     * @brief Add a training example to the training set of the learning module managed by this
     *control center window.
     * @param fs The feature set.
     * @param lbl_fps_br_priority The label for the FPS vs. Bitrate trainer.
     * @param lbl_size_quality_priority The label for the size vs. quality trainer.
     */
    virtual void addTrainingExample(FeatureSet &fs, double lbl_fps_br_priority, double lbl_size_quality_priority);

    /**
     * @brief Train the trainers.
     */
    virtual void train();

    /**
     * @brief usingStandardVideo
     * @return Whether or not this experiment is using standard video.
     */
    virtual bool usingStandardVideo();

    /**
     * @brief Make a prediction using the given feature set.
     * @param fs The feature set to make a prediction on.
     * @return A bitmask indicating the classes chosen from the trainers used.
     */
    virtual int predict(FeatureSet &fs);
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

    //Flag indicating what kind of test this is
    bool                    m_standard_video_experiment;
    //Functions
    void addCameras(CameraList cameras);

    //To test the demuxer
#if !PLAY_WITH_VLC
    FFMPEGWrapper           *m_ffmpeg;
#endif
    QThread                 *m_ffmpeg_thread;

    //Machine learning interface
    //FPS vs. Bitrate priority learner
    LearningInterface       m_fps_bitrate_learning_module;

    //Size vs. Quality priority learner
    LearningInterface       m_size_quality_learning_module;

    //FOR TESTING
    void testLearning();

    //Get a training set from file
    void loadTrainingSet();

};

#endif // CONTROLCENTER_H
