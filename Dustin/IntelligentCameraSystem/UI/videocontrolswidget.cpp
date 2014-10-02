#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>
#include "Util/FileUtil/filereaderutility.h"
#include "videocontrolswidget.h"

VideoControlsWidget::VideoControlsWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set up the UI
    this->setUpUI();
    //this->fadeOut();

    //setPalette(Qt::transparent);
    //setAttribute(Qt::WA_TransparentForMouseEvents);
}

VideoControlsWidget::~VideoControlsWidget()
{
    //DESTROY!!!!
#if !USE_COMBO_BOX
    delete lblWidth;
    delete lblHeight;
    delete leWidth;
    delete leHeight;
    delete leFPS;
#else
    delete lblWidthHeight;
    delete cbResolution;
    delete cbFps;
#endif
    delete lblFPS;


    delete bSend;

}

//void VideoControlsWidget::enterEvent(QEvent *e)
//{
//    QWidget::enterEvent(e);
//    //show the widget
//    this->fadeIn();
//}

//void VideoControlsWidget::leaveEvent(QEvent *e)
//{
//    QWidget::leaveEvent(e);
//    //hide
//    this->fadeOut();
//}
//Overloaded paint event (to use stylesheet)
void VideoControlsWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
//Slot for when send button is clicked
void VideoControlsWidget::sendButtonClicked()
{
    //emit sendClicked();
    //Determine the width field and height field
    QString width("");
    QString height("");
    QString fps("");
#if !USE_COMBO_BOX
    width = this->leWidth->text();
    height = this->leHeight->text();
    fps = leFPS->text();
#else
    QStringList res = this->cbResolution->currentText().split("x");
    width = res.at(0);
    height = res.at(1);
    fps = this->cbFps->currentText();
#endif

    //Check to see if all of the fields have been filled
    if(width == "" || height == "" || fps == "" || this->lebitRate->text() == "")
    {
        //Create a message box to indicate error
        QMessageBox msgBox;
        msgBox.setText("All fields must be filled!");
        int ret = msgBox.exec();

    }
    else
    {
        //Inform the parent to resize the video
        emit sendClicked(width, height, fps, lebitRate->text());
    }
}

//Called from a timer, delays hide event
void VideoControlsWidget::hideDelay()
{
    this->hide();
}

void VideoControlsWidget::setUpUI()
{
    //instantiate the labels
#if !USE_COMBO_BOX
    this->lblWidth = new QLabel("W:",this);
    this->lblHeight = new QLabel("H:",this);
#else
    this->lblWidthHeight = new QLabel("WxH:", this);
#endif

    this->lblFPS = new QLabel("FPS:",this);
    this->lblbitRate = new QLabel("Bps:",this);

    //instantiate the line edits
#if !USE_COMBO_BOX
    this->leWidth = new QLineEdit(this);
    this->leHeight = new QLineEdit(this);
    this->leFPS = new QLineEdit(this);
#else
    //Instantiate the combo box
    this->cbResolution = new QComboBox(this);
    //Populate the combo box
    QByteArray pathname = qgetenv("INTELLIGENT_CAMERA_SYSTEM_ROOT_DIR");
    QString filename(pathname);
    filename += "/";
    filename += STANDARD_RESOLUTIONS_FILE;
    FileReaderUtility reader(filename);
    if(reader.open_file())
    {
        this->all_sizes = reader.parse();
        this->cbResolution->addItems(all_sizes);
    }
    else
    {
        //If file fails to open... do something (will determine)

    }
    this->cbFps = new QComboBox(this);
    this->cbFps->addItem("30");
    this->cbFps->addItem("15");
#endif

    this->lebitRate = new QLineEdit(this);




    //Instantiate the SEND button
    this->bSend = new QPushButton("SEND", this);
    //this->bSend->setStyleSheet("border: 1px solid #1b1b1b; border-radius: 2px");
    this->setFocusPolicy(Qt::NoFocus);
    connect(bSend, SIGNAL(clicked()), this, SLOT(sendButtonClicked()));
    //Set up the layouts
#if !USE_COMBO_BOX
    QHBoxLayout *widths = new QHBoxLayout;
    widths->addWidget(lblWidth);
    widths->addWidget(leWidth);

    QHBoxLayout *heights = new QHBoxLayout;
    heights->addWidget(lblHeight);
    heights->addWidget(leHeight);

    QHBoxLayout *FPSLayout = new QHBoxLayout;
    FPSLayout->addWidget(lblFPS);
    FPSLayout->addWidget(leFPS);
#else
    QHBoxLayout *resolution = new QHBoxLayout;
    resolution->addWidget(lblWidthHeight);
    resolution->addWidget(cbResolution);

    QHBoxLayout *FPSLayout = new QHBoxLayout;
    FPSLayout->addWidget(lblFPS);
    FPSLayout->addWidget(cbFps);
#endif


    QHBoxLayout *BPSLayout = new QHBoxLayout;
    BPSLayout->addWidget(lblbitRate);
    BPSLayout->addWidget(lebitRate);


    //Main layout
    QHBoxLayout *layout = new QHBoxLayout;
#if !USE_COMBO_BOX
    layout->addLayout(widths);
    layout->addLayout(heights);
#else
    layout->addLayout(resolution);
#endif
    layout->addLayout(FPSLayout);
    layout->addLayout(BPSLayout);
    //layout->addSpacing(20);
    layout->addWidget(bSend);

//    QHBoxLayout *button = new QHBoxLayout;
//    button->addSpacing(300);
//    button->addWidget(bSend);


//    QVBoxLayout *main = new QVBoxLayout;
//    main->addLayout(layout);
//    main->addLayout(button);

    this->setLayout(layout);


}
bool VideoControlsWidget::use_standard_video() const
{
    return m_use_standard_video;
}

void VideoControlsWidget::setUse_standard_video(bool use_standard_video)
{
    m_use_standard_video = use_standard_video;
}

//Function to fade out
void VideoControlsWidget::fadeOut()
{
    QGraphicsOpacityEffect *fade_out_effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(fade_out_effect);
    QPropertyAnimation *fade_out_animation = new QPropertyAnimation(fade_out_effect, "opacity");
    fade_out_animation->setEasingCurve(QEasingCurve::InOutQuad);
    fade_out_animation->setDuration(175);
    fade_out_animation->setStartValue(0.8);
    fade_out_animation->setEndValue(0.0);

    fade_out_animation->start(QPropertyAnimation::DeleteWhenStopped);
    //this->hide();
    QTimer::singleShot(225, this, SLOT(hideDelay()));

}

//Function to fade in
void VideoControlsWidget::fadeIn()
{
    this->show();
    QGraphicsOpacityEffect *fade_in_effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(fade_in_effect);
    QPropertyAnimation *fade_in_animation = new QPropertyAnimation(fade_in_effect, "opacity");
    fade_in_animation->setEasingCurve(QEasingCurve::InOutQuad);
    fade_in_animation->setDuration(250);
    fade_in_animation->setStartValue(0.1);
    fade_in_animation->setEndValue(0.8);

    fade_in_animation->start(QPropertyAnimation::DeleteWhenStopped);

}

QStringList &VideoControlsWidget::getSizes()
{
    return this->all_sizes;
}
