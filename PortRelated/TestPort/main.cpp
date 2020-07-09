//#include <QCoreApplication>
#include <iostream>

#include "myserialport.h"


int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    std::string portName;
    if(argc < 2)
    {
        portName = "COM2";
    }
    else
    {
        portName = argv[1];
    }

    std::cout << portName << "\n";


    MySerialPort *newPort = new MySerialPort();
    newPort->func1(portName);
    newPort->func2();

    //Sleep(5000);
    //newPort->stopMonitorThread = true;

    for(;;)
    {
        if(newPort->exitProcess)
        {
            break;
        }

        Sleep(50);
    }

    delete newPort;
    newPort = NULL;

    std::cout << "exit 0";

    //return a.exec();  //这种写法会导致进程无法退出
    return 0;
}
