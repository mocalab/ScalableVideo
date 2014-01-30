/********************************************************************************
** Form generated from reading UI file 'serverpropertiesdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERPROPERTIESDIALOG_H
#define UI_SERVERPROPERTIESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerPropertiesDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblHost;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *leHost;
    QLineEdit *lePort;
    QCheckBox *cbMakeDefault;

    void setupUi(QDialog *ServerPropertiesDialog)
    {
        if (ServerPropertiesDialog->objectName().isEmpty())
            ServerPropertiesDialog->setObjectName(QStringLiteral("ServerPropertiesDialog"));
        ServerPropertiesDialog->resize(400, 213);
        buttonBox = new QDialogButtonBox(ServerPropertiesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 170, 341, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(ServerPropertiesDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 361, 112));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lblHost = new QLabel(layoutWidget);
        lblHost->setObjectName(QStringLiteral("lblHost"));

        verticalLayout_2->addWidget(lblHost);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        leHost = new QLineEdit(layoutWidget);
        leHost->setObjectName(QStringLiteral("leHost"));

        verticalLayout->addWidget(leHost);

        lePort = new QLineEdit(layoutWidget);
        lePort->setObjectName(QStringLiteral("lePort"));
        lePort->setMaxLength(6);

        verticalLayout->addWidget(lePort);


        horizontalLayout->addLayout(verticalLayout);

        cbMakeDefault = new QCheckBox(ServerPropertiesDialog);
        cbMakeDefault->setObjectName(QStringLiteral("cbMakeDefault"));
        cbMakeDefault->setGeometry(QRect(10, 150, 121, 22));

        retranslateUi(ServerPropertiesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ServerPropertiesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ServerPropertiesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ServerPropertiesDialog);
    } // setupUi

    void retranslateUi(QDialog *ServerPropertiesDialog)
    {
        ServerPropertiesDialog->setWindowTitle(QApplication::translate("ServerPropertiesDialog", "Server Config", 0));
        lblHost->setText(QApplication::translate("ServerPropertiesDialog", "Host Name:", 0));
        label_2->setText(QApplication::translate("ServerPropertiesDialog", "Port:", 0));
        leHost->setText(QApplication::translate("ServerPropertiesDialog", "localhost", 0));
        lePort->setInputMask(QString());
        lePort->setText(QString());
        cbMakeDefault->setText(QApplication::translate("ServerPropertiesDialog", "Make Default ", 0));
    } // retranslateUi

};

namespace Ui {
    class ServerPropertiesDialog: public Ui_ServerPropertiesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERPROPERTIESDIALOG_H
