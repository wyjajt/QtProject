#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>


#include "MySerialPort.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void threadMonitorAndExitProcess();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    MySerialPort *testport;
};

#endif // MAINWINDOW_H
