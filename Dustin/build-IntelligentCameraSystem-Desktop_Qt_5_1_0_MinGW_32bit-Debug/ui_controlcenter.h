/********************************************************************************
** Form generated from reading UI file 'controlcenter.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLCENTER_H
#define UI_CONTROLCENTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlCenter
{
public:
    QAction *actionQuit;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *lblCameras;
    QListWidget *list_boxCameras;
    QVBoxLayout *verticalLayout_3;
    QPushButton *bOpen;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblOpenedCams;
    QListWidget *listbox_Opened;
    QCheckBox *cbLearningMode;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ControlCenter)
    {
        if (ControlCenter->objectName().isEmpty())
            ControlCenter->setObjectName(QStringLiteral("ControlCenter"));
        ControlCenter->resize(640, 480);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ControlCenter->sizePolicy().hasHeightForWidth());
        ControlCenter->setSizePolicy(sizePolicy);
        actionQuit = new QAction(ControlCenter);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionQuit->setMenuRole(QAction::QuitRole);
        centralwidget = new QWidget(ControlCenter);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 91, 625, 219));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lblCameras = new QLabel(layoutWidget);
        lblCameras->setObjectName(QStringLiteral("lblCameras"));

        verticalLayout->addWidget(lblCameras);

        list_boxCameras = new QListWidget(layoutWidget);
        list_boxCameras->setObjectName(QStringLiteral("list_boxCameras"));
        list_boxCameras->setEditTriggers(QAbstractItemView::NoEditTriggers);
        list_boxCameras->setAlternatingRowColors(true);
        list_boxCameras->setSortingEnabled(true);

        verticalLayout->addWidget(list_boxCameras);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        bOpen = new QPushButton(layoutWidget);
        bOpen->setObjectName(QStringLiteral("bOpen"));
        bOpen->setFocusPolicy(Qt::NoFocus);

        verticalLayout_3->addWidget(bOpen);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lblOpenedCams = new QLabel(layoutWidget);
        lblOpenedCams->setObjectName(QStringLiteral("lblOpenedCams"));

        verticalLayout_2->addWidget(lblOpenedCams);

        listbox_Opened = new QListWidget(layoutWidget);
        listbox_Opened->setObjectName(QStringLiteral("listbox_Opened"));
        listbox_Opened->setSortingEnabled(true);

        verticalLayout_2->addWidget(listbox_Opened);


        horizontalLayout->addLayout(verticalLayout_2);

        cbLearningMode = new QCheckBox(centralwidget);
        cbLearningMode->setObjectName(QStringLiteral("cbLearningMode"));
        cbLearningMode->setEnabled(false);
        cbLearningMode->setGeometry(QRect(370, 350, 121, 21));
        cbLearningMode->setChecked(true);
        ControlCenter->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ControlCenter);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        ControlCenter->setMenuBar(menubar);
        statusbar = new QStatusBar(ControlCenter);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ControlCenter->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(ControlCenter);

        QMetaObject::connectSlotsByName(ControlCenter);
    } // setupUi

    void retranslateUi(QMainWindow *ControlCenter)
    {
        ControlCenter->setWindowTitle(QApplication::translate("ControlCenter", "Intelligent Camera System Control Center", 0));
        actionQuit->setText(QApplication::translate("ControlCenter", "Quit", 0));
        actionQuit->setShortcut(QString());
        lblCameras->setText(QApplication::translate("ControlCenter", "Available Cameras:", 0));
        bOpen->setText(QApplication::translate("ControlCenter", "Open ->", 0));
        lblOpenedCams->setText(QApplication::translate("ControlCenter", "Opened Cameras:", 0));
        cbLearningMode->setText(QApplication::translate("ControlCenter", "Learning Mode", 0));
        menuFile->setTitle(QApplication::translate("ControlCenter", "&File", 0));
    } // retranslateUi

};

namespace Ui {
    class ControlCenter: public Ui_ControlCenter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLCENTER_H
