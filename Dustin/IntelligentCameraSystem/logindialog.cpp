#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "controlcenter.h"
#include <Util/FileUtil/cameralistfileutil.h>

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
//        //Would get this list from a web service call
        CameraListFileUtil camera_list_reader;
        QByteArray pathname = qgetenv("INTELLIGENT_CAMERA_SYSTEM_ROOT_DIR");
        QString filename(pathname);
        filename += "/";
        filename += CAMERAFILE_PATH;

        //Load the document
        if(!camera_list_reader.loadDocument(filename))
        {
            //Show a message box indicating the document couldn's be loaded
            QMessageBox *msgBox = new QMessageBox;
            msgBox->setWindowTitle("Error");
            msgBox->setText("Failed to load list of cameras (did you delete the configuration file?)");
            msgBox->setWindowModality(Qt::NonModal);
            msgBox->setAttribute(Qt::WA_DeleteOnClose);
            msgBox->show();
        }


        CameraList cameras = camera_list_reader.getCameraList();


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
