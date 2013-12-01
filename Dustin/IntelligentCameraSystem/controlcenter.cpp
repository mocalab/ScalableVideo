
#include "controlcenter.h"
#include "ui_controlcenter.h"
#include <QShortcut>

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

    //Set up demuxer test
    m_ffmpeg = new FFMPEGWrapper(39082);
    m_ffmpeg_thread = new QThread(this);
    m_ffmpeg->moveToThread(m_ffmpeg_thread);
    connect(m_ffmpeg_thread, SIGNAL(started()), m_ffmpeg, SLOT(demux()));
    connect(m_ffmpeg, SIGNAL(finished()), m_ffmpeg_thread, SLOT(quit()));

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
