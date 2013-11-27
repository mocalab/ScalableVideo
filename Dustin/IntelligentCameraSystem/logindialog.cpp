#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "controlcenter.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_bLogin_clicked()
{
    //Authenticate, get a list of the available cameras
    //The current plan is to accomplish this via a web service
    //For now we will just allow a test account and create dummy cameras
    QString username = this->ui->leUsername->text();
    QString password = this->ui->lePassword->text() ;
    if(username == "test" && password == "password")
    {
        //Would get this list from a web service call
        Address a("localhost", 37986);
        QString q("Camera A");

        Address ab("140.154.1.28", 7744);
        QString qb("DragonBoard");

        CameraList cameras;
        cameras.push_front(new Camera(q, a, CONTENT_TYPE_NONE));
        cameras.push_front(new Camera(qb, ab, CONTENT_TYPE_PERSONAL));

        ControlCenter *w = new ControlCenter(cameras);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();

        this->close();
    }
    else
    {
        //Incorrect login
        QMessageBox msgBox;
        msgBox.setText("Invalid credentials");
        (void) msgBox.exec();
    }
}

void LoginDialog::on_bQuit_clicked()
{
    //Quit the application
    this->close();
}
