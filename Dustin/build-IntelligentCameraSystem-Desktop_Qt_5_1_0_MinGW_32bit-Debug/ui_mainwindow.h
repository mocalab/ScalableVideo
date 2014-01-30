/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "UI/streamertextedit.h"
#include "UI/vlcvideowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionServer_Properties;
    QWidget *centralWidget;
    QPushButton *bSend;
    QPushButton *bConnect;
    StreamerTextEdit *teCommand;
    VLCVideoWidget *video_container;
    QLabel *lblFormat;
    QMenuBar *menuBar;
    QMenu *menuOptions;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 720);
        actionServer_Properties = new QAction(MainWindow);
        actionServer_Properties->setObjectName(QStringLiteral("actionServer_Properties"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        bSend = new QPushButton(centralWidget);
        bSend->setObjectName(QStringLiteral("bSend"));
        bSend->setGeometry(QRect(250, 100, 101, 27));
        bConnect = new QPushButton(centralWidget);
        bConnect->setObjectName(QStringLiteral("bConnect"));
        bConnect->setGeometry(QRect(360, 10, 151, 27));
        teCommand = new StreamerTextEdit(centralWidget);
        teCommand->setObjectName(QStringLiteral("teCommand"));
        teCommand->setGeometry(QRect(10, 10, 345, 86));
        video_container = new VLCVideoWidget(centralWidget);
        video_container->setObjectName(QStringLiteral("video_container"));
        video_container->setGeometry(QRect(10, 130, 511, 451));
        lblFormat = new QLabel(centralWidget);
        lblFormat->setObjectName(QStringLiteral("lblFormat"));
        lblFormat->setGeometry(QRect(360, 80, 251, 17));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 25));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOptions->menuAction());
        menuOptions->addAction(actionServer_Properties);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Command Center", 0));
        actionServer_Properties->setText(QApplication::translate("MainWindow", "Server Properties", 0));
        bSend->setText(QApplication::translate("MainWindow", "Send", 0));
        bConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        lblFormat->setText(QApplication::translate("MainWindow", "URL Format: 'udp/h264://@:<port>'", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
