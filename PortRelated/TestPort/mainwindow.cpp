#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    testport = new MySerialPort();
}

MainWindow::~MainWindow()
{
    if(!testport)
    {
        delete testport;
        testport = NULL;
    }

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    testport->func1();

    testport->func2();

    std::thread td = std::thread(&MainWindow::threadMonitorAndExitProcess, this);
    td.detach();
}

void MainWindow::threadMonitorAndExitProcess()
{
    for(;;)
    {
        if(testport->exitProcess)
        {
            delete testport;
            testport = NULL;

            break;
        }

        Sleep(50);
    }


    reject();
}
