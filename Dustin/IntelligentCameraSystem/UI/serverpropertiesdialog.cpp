#include "serverpropertiesdialog.h"
#include "ui_serverpropertiesdialog.h"
#include "Util/FileUtil/serverconfigfileutil.h"

ServerPropertiesDialog::ServerPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerPropertiesDialog),
    m_file_util(new ServerConfigFileUtil),
    m_file_name(QString("/home/dustin/QtWorkspace/misc-applications/VideoStreamController/Config/server_config.xml"))
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(acceptedClicked()));
    //Attempt to load server properties
    bool loaded = m_file_util->loadDocument(m_file_name.toStdString().c_str());
    if(loaded)
    {
        AddressList servers = m_file_util->getServerInfo();
        AddressListIter first_node = servers.begin();
        m_port = first_node->getPort();
        m_host_name = QString(first_node->getAddress().c_str());
        ui->leHost->setText(m_host_name);
        ui->lePort->setText(QString::number(m_port));

    }
}

ServerPropertiesDialog::~ServerPropertiesDialog()
{
    delete ui;
    delete m_file_util;
}

QString ServerPropertiesDialog::getHostName() const
{
    return m_host_name;
}
int ServerPropertiesDialog::getPort() const
{
    return m_port;
}

void ServerPropertiesDialog::acceptedClicked()
{
    //Set the new properties
    this->m_host_name = ui->leHost->text();
    this->m_port = ui->lePort->text().toInt();

    if(ui->cbMakeDefault->isChecked())
    {
        //Save to xml file

        //Using list as will have multiple in future
        AddressList addresses;
        addresses.push_back(Address(m_host_name.toStdString(), m_port));

        m_file_util->saveServerInfo(addresses, m_file_name.toStdString().c_str());
    }
}

