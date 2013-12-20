
#include "controlcenter.h"
#include "ui_controlcenter.h"
#include <QShortcut>
#include "Types/featureset.h"
#include <QMessageBox>

ControlCenter::ControlCenter(CameraList cameras, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlCenter)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    //c = new Camera(q, a);

    this->addCameras(cameras);

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quitActionClicked()));
    QShortcut *quitHotkey = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(quitHotkey, SIGNAL(activated()), this, SLOT(quitActionClicked()));

#if !PLAY_WITH_VLC
    //Set up demuxer test
    m_ffmpeg = new FFMPEGWrapper(39082);
    m_ffmpeg_thread = new QThread(this);
    m_ffmpeg->moveToThread(m_ffmpeg_thread);
    connect(m_ffmpeg_thread, SIGNAL(started()), m_ffmpeg, SLOT(demux()));
    connect(m_ffmpeg, SIGNAL(finished()), m_ffmpeg_thread, SLOT(quit()));
#endif
    //Uncomment to run learning algorithm tests
   //this->testLearning();

}

ControlCenter::~ControlCenter()
{
    delete ui;

}

void ControlCenter::on_bOpen_clicked()
{
    QList<QListWidgetItem *> items = ui->list_boxCameras->selectedItems();
    if(!items.isEmpty())
    {
        QListWidgetItem *selected = ui->list_boxCameras->takeItem(ui->list_boxCameras->row(items.first()));//ui->list_boxCameras->selectedItems().first();
        //ui->list_boxCameras->removeItemWidget(selected);
        ui->listbox_Opened->addItem(selected);

        //Open up a new camera window
        Camera *cam = (Camera *)selected;
        VideoWindow *openCamera = new VideoWindow(cam, this);
        openCamera->setAttribute(Qt::WA_DeleteOnClose);
        openCamera->show();

        connect(openCamera, SIGNAL(sendCamera(Camera*)), this, SLOT(videoWindowClosed(Camera*)));
    }
//    if((*c) == (*selected))
//    {
//        bool test = true;
    //    }
}

void ControlCenter::videoWindowClosed(Camera *cam)
{
    QListWidgetItem *closed = ui->listbox_Opened->takeItem(ui->listbox_Opened->row(cam));
    ui->list_boxCameras->addItem(closed);
}

void ControlCenter::quitActionClicked()
{
    this->close();
}

//Adds the cameras in cameras list to the available cameras list box
void ControlCenter::addCameras(CameraList cameras)
{
    for(CameraListIterator iter = cameras.begin(); iter != cameras.end(); iter++)
    {
        ui->list_boxCameras->addItem(*iter);
    }
}

void ControlCenter::testLearning()
{
    //TESTING
//    FeatureSet fs1(20000, 1);
//    FeatureSet fs2(700000, 1);
//    FeatureSet fs3(100000, 1);
//    FeatureSet fs4(600000, 1);
//    FeatureSet fs5(900000, 1);
//    FeatureSet fs6(800000, 1);
//    FeatureSet fs7(100000, 1);
//    FeatureSet fs8(200000, 1);
//    FeatureSet fs9(400000, 1);
//    FeatureSet fs10(500000, 1);
//    FeatureSet fs11(100000, 1);
//    FeatureSet fs12(700000, 1);
//    FeatureSet fs13(100000, 1);
//    FeatureSet fs14(600000, 1);
//    FeatureSet fs15(700000, 1);
//    FeatureSet fs16(500000, 1);
//    FeatureSet fs17(100000, 1);
//    FeatureSet fs18(200000, 1);
//    FeatureSet fs19(100000, 1);
//    FeatureSet fs20(900000, 1);

//    m_fps_bitrate_learning_module.addTrainingSample(fs1, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs2, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs3, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs4, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs5, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs6, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs7, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs8, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs9, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs10, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs11, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs12, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs13, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs14, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs15, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs16, -1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs17, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs18, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs19, 1.0);
//    m_fps_bitrate_learning_module.addTrainingSample(fs20, -1.0);
    for (int r = -20; r <= 20; ++r)
       {
           for (int c = -20; c <= 20; ++c)
           {
               FeatureSet fs(r,c);
               double label = 0.0;
               // if this point is less than 10 from the origin
               if (sqrt((double)r*r + c*c) <= 10)
                   label = 1.0;
               else
                   label = -1.0;
                m_fps_bitrate_learning_module.addTrainingSample(fs, label);
           }
       }


    m_fps_bitrate_learning_module.trainCurrent();

    //TEST SOME EXAMPLES
    double out = -10.0;
    FeatureSet tst1(1, 1);
    out = m_fps_bitrate_learning_module.predict(tst1);

    FeatureSet tst2(10, 10);
    out = m_fps_bitrate_learning_module.predict(tst2);
    out = 5.0;
    return;
}

void ControlCenter::on_b_demux_test_clicked()
{
    m_ffmpeg_thread->start();
}

bool ControlCenter::inLearningMode()
{
    //For now, just return the value of the learning mode check box. In the future, we will also use the number
    //of training samples we have
    return this->ui->cbLearningMode->isChecked();
}

//Add a training example
void ControlCenter::addTrainingExample(FeatureSet &fs, double lbl_fps_br_priority, double lbl_size_quality_priority)
{
    //Add to fps/br trainer
    this->m_fps_bitrate_learning_module.addTrainingSample(fs, lbl_fps_br_priority);

    //Add to quality trainer
    this->m_size_quality_learning_module.addTrainingSample(fs, lbl_size_quality_priority);

    if((m_size_quality_learning_module.getNumberOfSamples() % 20) == 0)
    {
        this->train();

        if(!this->ui->cbLearningMode->isEnabled())
            this->ui->cbLearningMode->setEnabled(true);
    }
}

//Train the learning modules
void ControlCenter::train()
{
    //Train fps/br
    this->m_fps_bitrate_learning_module.trainCurrent();

    //Train quality
    this->m_size_quality_learning_module.trainCurrent();
}

//Predict the class based on the given feature set
int ControlCenter::predict(FeatureSet &fs)
{
    //Run the prediction on both trainers and create a bitmask
    int fps_bitrate = this->m_fps_bitrate_learning_module.predict(fs) >= 0.0 ? 1 : 0;
    int quality_size = this->m_size_quality_learning_module.predict(fs) >= 0.0 ? 1 : 0;
    return quality_size | (fps_bitrate << 1);
}
