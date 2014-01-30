/********************************************************************************
** Form generated from reading UI file 'videowindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOWINDOW_H
#define UI_VIDEOWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include "UI/videocontrolswidget.h"
#include "UI/vlcvideowidget.h"

QT_BEGIN_NAMESPACE

class Ui_VideoWindow
{
public:
    VLCVideoWidget *video_player;
    VideoControlsWidget *video_controls;

    void setupUi(QDialog *VideoWindow)
    {
        if (VideoWindow->objectName().isEmpty())
            VideoWindow->setObjectName(QStringLiteral("VideoWindow"));
        VideoWindow->resize(320, 240);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VideoWindow->sizePolicy().hasHeightForWidth());
        VideoWindow->setSizePolicy(sizePolicy);
        VideoWindow->setCursor(QCursor(Qt::ArrowCursor));
        VideoWindow->setMouseTracking(true);
        VideoWindow->setFocusPolicy(Qt::ClickFocus);
        VideoWindow->setSizeGripEnabled(false);
        VideoWindow->setModal(false);
        video_player = new VLCVideoWidget(VideoWindow);
        video_player->setObjectName(QStringLiteral("video_player"));
        video_player->setGeometry(QRect(0, 0, 320, 240));
        video_player->setFocusPolicy(Qt::StrongFocus);
        video_player->setStyleSheet(QStringLiteral("background-color: #000000;"));
        video_controls = new VideoControlsWidget(VideoWindow);
        video_controls->setObjectName(QStringLiteral("video_controls"));
        video_controls->setGeometry(QRect(10, 10, 320, 45));

        retranslateUi(VideoWindow);

        QMetaObject::connectSlotsByName(VideoWindow);
    } // setupUi

    void retranslateUi(QDialog *VideoWindow)
    {
        VideoWindow->setWindowTitle(QApplication::translate("VideoWindow", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class VideoWindow: public Ui_VideoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOWINDOW_H
