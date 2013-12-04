#include "mainwindow.h"
#include "networkinterface.h"
#include "ui_mainwindow.h"
#include "CollectedStatistics.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_start,SIGNAL(clicked()),this,SLOT(startButtonClick()));
    connect(ui->pushButton_refreshInterfaceList,SIGNAL(clicked()),this,SLOT(refreshInterfaceListButtonClick()));
    connect(ui->spinBox_packetLoss1,SIGNAL(editingFinished()),this,SLOT(lossRateChanged()));
    connect(ui->spinBox_packetLoss2,SIGNAL(editingFinished()),this,SLOT(lossRateChanged()));
    connect(ui->doubleSpinBox_bandwith1,SIGNAL(editingFinished()),this,SLOT(bandWidthChanged()));
    connect(ui->doubleSpinBox_bandwith2,SIGNAL(editingFinished()),this,SLOT(bandWidthChanged()));

    networkBridge = new NetworkEmulator(this);
    connect(networkBridge,SIGNAL(addAdapterInterface(QString)),this,SLOT(addAdapterInterface(QString)));
    connect(networkBridge,SIGNAL(statisticsCollected(CollectedStatistics*)),this,SLOT(receiveStatistics(CollectedStatistics*)));

    networkBridge->lookupAdapterInterfaces();
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    started = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveStatistics(CollectedStatistics* stats)
{
    ui->label_sent1->setText(tr("Packets Read: %1").arg(stats->packetsSent1));
    ui->label_sent2->setText(tr("Packets Read: %1").arg(stats->packetsSent2));

    ui->label_dropped1->setText(tr("Packets Dropped: %1").arg(stats->packetsDropped1));
    ui->label_dropped2->setText(tr("Packets Dropped: %1").arg(stats->packetsDropped2));

    ui->label_bandwidth1->setText(tr("Actual Bandwith: %1 Mb/S").arg(stats->bandwidth1));
    ui->label_bandwidth2->setText(tr("Actual Bandwith: %1 Mb/S").arg(stats->bandwidth2));

    ui->label_buffer1->setText(tr("Buffer Size: %1").arg(stats->bufferSize1));
    ui->label_buffer2->setText(tr("Buffer Size: %1").arg(stats->bufferSize2));
}

void MainWindow::lossRateChanged()
{
    int lossRate1 = ui->spinBox_packetLoss1->value();
    int lossRate2 = ui->spinBox_packetLoss2->value();
    networkBridge->setLossrate(lossRate1,lossRate2);
}

void MainWindow::bandWidthChanged()
{
    double bandwidth1 = ui->doubleSpinBox_bandwith1->value();
    double bandwidth2 = ui->doubleSpinBox_bandwith2->value();
    networkBridge->setBandwidth(bandwidth1,bandwidth2);
}

void MainWindow::refreshInterfaceListButtonClick()
{
    ui->comboBox_adapter1->clear();
    ui->comboBox_adapter2->clear();
    networkBridge->lookupAdapterInterfaces();
}

void MainWindow::addAdapterInterface(QString interfaceDescription)
{
    ui->comboBox_adapter1->addItem(interfaceDescription);
    ui->comboBox_adapter2->addItem(interfaceDescription);

    if ( ui->comboBox_adapter2->currentIndex() == 0 && ui->comboBox_adapter2->count() > 1 )
        ui->comboBox_adapter2->setCurrentIndex(1);
}


void MainWindow::startButtonClick()
{
    if ( started )
    {
        started = false;
        ui->pushButton_start->setText("Start");
        ui->comboBox_adapter1->setEnabled(true);
        ui->comboBox_adapter2->setEnabled(true);
        ui->lineEdit_macFilter1->setEnabled(true);
        ui->lineEdit_macFilter2->setEnabled(true);
        ui->pushButton_refreshInterfaceList->setEnabled(true);
        networkBridge->Stop();
    }
    else
    {
        started = true;
        ui->pushButton_start->setText("Stop");
        ui->comboBox_adapter1->setEnabled(false);
        ui->comboBox_adapter2->setEnabled(false);
        ui->lineEdit_macFilter1->setEnabled(false);
        ui->lineEdit_macFilter2->setEnabled(false);
        ui->pushButton_refreshInterfaceList->setEnabled(false);


        int selectedIndexOne = ui->comboBox_adapter1->currentIndex();
       // QString nameInterfaceOne = ui->comboBox_adapter1->itemData(selectedIndexOne).toString();
        QString macFilterOne = ui->lineEdit_macFilter1->text();
        int lossRate1 = ui->spinBox_packetLoss1->value();

        int selectedIndexTwo = ui->comboBox_adapter2->currentIndex();
      //  QString nameInterfaceTwo = ui->comboBox_adapter2->itemData(selectedIndexTwo).toString();
        QString macFilterTwo = ui->lineEdit_macFilter2->text();
        int lossRate2 = ui->spinBox_packetLoss2->value();

        double bandwidth1=ui->doubleSpinBox_bandwith1->value();
        double bandwidth2=ui->doubleSpinBox_bandwith2->value();

        networkBridge->setSelectedInterfaces(selectedIndexOne,selectedIndexTwo);
        networkBridge->setMacFilters(macFilterOne,macFilterTwo);
        networkBridge->setLossrate(lossRate1,lossRate2);
        networkBridge->setBandwidth(bandwidth1,bandwidth2);
        networkBridge->startBridge();
    }
}
