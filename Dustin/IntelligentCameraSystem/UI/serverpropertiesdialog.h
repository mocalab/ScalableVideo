#ifndef SERVERPROPERTIESDIALOG_H
#define SERVERPROPERTIESDIALOG_H

#include <QDialog>
#include <Util/FileUtil/serverconfigfileutil.h>

namespace Ui {
class ServerPropertiesDialog;
}

/**
 * @brief Dialog box to set server properties.
 * @deprecated This widget was used in an old version of the UI and is now no longer present.
 */
class ServerPropertiesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ServerPropertiesDialog(QWidget *parent = 0);
    ~ServerPropertiesDialog();

    //Accessors
    QString getHostName() const;
    int getPort() const;
private slots:
    void acceptedClicked();

private:
    Ui::ServerPropertiesDialog *ui;

    //Placeholders for text field values
    QString                     m_host_name;
    int                         m_port;

    //Initial loader file
    ServerConfigFileUtil        *m_file_util;
    const QString               m_file_name;
};

#endif // SERVERPROPERTIESDIALOG_H
