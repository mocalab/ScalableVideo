/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblUsername;
    QLabel *lblPassword;
    QVBoxLayout *verticalLayout;
    QLineEdit *leUsername;
    QLineEdit *lePassword;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *bQuit;
    QPushButton *bLogin;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QStringLiteral("LoginDialog"));
        LoginDialog->resize(400, 205);
        layoutWidget = new QWidget(LoginDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 50, 351, 91));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lblUsername = new QLabel(layoutWidget);
        lblUsername->setObjectName(QStringLiteral("lblUsername"));

        verticalLayout_2->addWidget(lblUsername);

        lblPassword = new QLabel(layoutWidget);
        lblPassword->setObjectName(QStringLiteral("lblPassword"));

        verticalLayout_2->addWidget(lblPassword);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        leUsername = new QLineEdit(layoutWidget);
        leUsername->setObjectName(QStringLiteral("leUsername"));

        verticalLayout->addWidget(leUsername);

        lePassword = new QLineEdit(layoutWidget);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(lePassword);


        horizontalLayout->addLayout(verticalLayout);

        layoutWidget1 = new QWidget(LoginDialog);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(180, 170, 191, 29));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        bQuit = new QPushButton(layoutWidget1);
        bQuit->setObjectName(QStringLiteral("bQuit"));

        horizontalLayout_2->addWidget(bQuit);

        bLogin = new QPushButton(layoutWidget1);
        bLogin->setObjectName(QStringLiteral("bLogin"));
        bLogin->setDefault(true);
        bLogin->setFlat(false);

        horizontalLayout_2->addWidget(bLogin);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Intelligent Camera System Login", 0));
        lblUsername->setText(QApplication::translate("LoginDialog", "Username:", 0));
        lblPassword->setText(QApplication::translate("LoginDialog", "Password:", 0));
        bQuit->setText(QApplication::translate("LoginDialog", "Quit", 0));
        bLogin->setText(QApplication::translate("LoginDialog", "Login", 0));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
