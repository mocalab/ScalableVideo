#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class NetworkEmulator;
struct CollectedStatistics;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addAdapterInterface(QString interfaceDescription);

private slots:
    void startButtonClick();
    void refreshInterfaceListButtonClick();
    void lossRateChanged();
    void bandWidthChanged();
    void receiveStatistics(CollectedStatistics* stats);

private:
    Ui::MainWindow *ui;
    NetworkEmulator* networkBridge;
    bool started;
};

#endif // MAINWINDOW_H
