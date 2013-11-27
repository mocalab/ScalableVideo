#include "QsLog.h"
#include "QsLogDest.h"
#include "mainwindow.h"
#include "controlcenter.h"
#include "logindialog.h"
#include "Global/Constants.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //INITIALIZE LOGGER
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::DebugLevel);

    //Get path to the logging directory; Developer should set this environment variable
    QByteArray pathname = qgetenv("INTELLIGENT_CAMERA_SYSTEM_ROOT_DIR");
    QString filename(pathname);
    filename += "/";
    filename += LOGFILE_PATH;
    const QString path(QDir(a.applicationDirPath()).filePath(filename));
    QsLogging::DestinationPtr dest(QsLogging::DestinationFactory::MakeFileDestination(path));
    logger.addDestination(dest.get());
    QLOG_INFO() << __FILE__ << ":" << __LINE__ << " -- " << "Program init.";
    //MainWindow *w = new MainWindow;
    //w->show();
//    ControlCenter *w = new ControlCenter;
//    w->show();
    LoginDialog *w = new LoginDialog;
    w->show();

    int ret = a.exec();
    delete w;
    return ret;
}

