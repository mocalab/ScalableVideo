#ifndef VIDEOCONTROLSWIDGET_H
#define VIDEOCONTROLSWIDGET_H
//Macro to define whether or not a combo box is used for resolution
#define USE_COMBO_BOX       01


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include "Global/Constants.h"


class VideoControlsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoControlsWidget(QWidget *parent = 0);
    ~VideoControlsWidget();
    void fadeOut();
    void fadeIn();

protected:
//    virtual void enterEvent(QEvent *e);
//    virtual void leaveEvent(QEvent *e);
    virtual void paintEvent(QPaintEvent *e);

signals:
    void sendClicked();
    void sendClicked(QString width, QString height, QString fps, QString bps);
public slots:
    void sendButtonClicked();
private slots:
    //To delay the hide on a timer
    void hideDelay();
private:
    void setUpUI();

    //UI Elements

    //Labels
    QLabel              *lblWidth;
    QLabel              *lblHeight;
    QLabel              *lblWidthHeight;
    QLabel              *lblFPS;
    QLabel              *lblbitRate;

    //Line Edits
    QLineEdit           *leWidth;
    QLineEdit           *leHeight;
    QLineEdit           *leFPS;
    QLineEdit           *lebitRate;

    //Combo box for Width x Height
    QComboBox           *cbResolution;

    //Button
    QPushButton         *bSend;
};

#endif // VIDEOCONTROLSWIDGET_H
