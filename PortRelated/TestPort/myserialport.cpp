#include "MySerialPort.h"

MySerialPort::MySerialPort()
{
    stopMonitorThread = false;
    exitProcess = false;
    pp.flag  = -1;
    pp.value = -1;

    InitLog("MySerialPort", "1.0.1");  //日志初始化
    Log_Debug( log_g, "MySerialPort start .." );
}


void MySerialPort::func1(std::string portName)
{
    QString port = QString::fromStdString(portName);
    SetPortName( port );
    SetPortBaudRate( 9600 );
    SetPortParity( 'N' );
    SetPortDataBits( 8 );
    SetPortStopBits( 1.0 );
    if( !OpenComPort() )  //打开串口失败
    {
        Log_Warn( log_g, "open port failed !!" );
        return;
    }

    Log_Debug( log_g, "SerialPort open success .." );

    Log_Debug( log_g, "Start create shareMemory .." );
    //1.创建共享文件句柄 hMapFile，CreateFileMapping()函数创建一个文件映射内核对象
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,     //物理文件句柄，设为INVALID_HANDLE_VALUE（无效句柄）以创建一个进程间共享的对象
        NULL,				      //默认安全级别
        PAGE_READWRITE,           //权限可读可写
        0,						  //高位文件大小
        sizeof(test),		  //低位文件大小
        cstrSzName				  //共享内存名
        );

    //2.获取指向文件视图的指针 pBuf，MapViewOfFile()函数负责把文件数据映射到进程的地址空间
    pBuf = (LPVOID *)MapViewOfFile(
        hMapFile,				  //共享内存的句柄
        FILE_MAP_ALL_ACCESS,      //可读写
        0,
        0,
        sizeof(test)
        );

    //pp置初值
    memcpy(pBuf, &pp, sizeof(test) + 10);
    Log_Debug( log_g, "Create shareMemory success .." );


}

void MySerialPort::func2()
{
    //判断缓冲区是否存在待读数据
    if( port->bytesAvailable() > 0 )
    {
        port->readAll();
    }

    std::thread threadMonitor = std::thread(&MySerialPort::threadMonitorPort, this);
    threadMonitor.detach();
}

void MySerialPort::threadMonitorPort()
{
    QByteArray cmd("*ADT00000a");

    Log_Debug( log_g , "Enter threadMonitorPort ...");
    long tm_start,tm_end;
    QByteArray  data_receive;
    data_receive.clear();

    for(;;)
    {
        //每次循环发送一次指令
        memcpy(&pp, (test *)pBuf, sizeof(test) + 10);
        if(stopMonitorThread || pp.flag == 2)
        {
            Log_Warn(log_g, "Stop threadMonitorPort !!");
            break;
        }

        if(pp.flag != 1)    //没收到开始测温指令
        {
            //Log_Debug(log_g, "..............................flag 0 ..");
            Delay_ms(50);
            continue;
        }

        data_receive = port->readAll();
        if(data_receive.size() > 0)
        {
            Log_Warn(log_g, "Extra feedback: %s", data_receive.data());
        }
        //port->readAll();        //清空串口接收区
        data_receive = "";
        if( port->write( cmd ) == cmd.size() )
        {
            //Log_Debug( log_g , "Send Command Success!");
            tm_start    =   clock();

            while( true )
            {
                if( port->bytesAvailable() > 0 )  //有反馈信号
                {
                    while( true )
                    {
                        data_receive += port->readAll();

                        if( port->bytesAvailable() <= 0 )
                        {
                            //Log_Debug( log_g , "Break!!!" );
                            break;
                        }
                        Delay_ms(1);
                    }
                }

                if( data_receive.size() > 8 )
                {
                    //Log_Debug( log_g , "Anlyz Rcv_data --> %s " , data_receive.data() );
                    std::string stdString = data_receive.data();
                    int startPos = stdString.find("*ADT");
                    int endPos = stdString.find('C', startPos + 1);
                    if (startPos >= 0 && (endPos - startPos == 8))
                    {
                        //Log_Debug(log_g, "Feedback: %s", data_receive.data());

                        char strNum[5] = {};
                        strncpy(strNum, stdString.data() + startPos + 4, 4);

                        memcpy(&pp, (test *)pBuf, sizeof(test) + 10);
                        if(pp.flag != 2)    //如果主进程已经置为2, 则这里不再写1
                        {
                            pp.value = atoi(strNum);    //温度数据写入共享内存
                            Log_Debug(log_g, "Temperature: %d", pp.value);
                            pp.flag = 0;
                            memcpy(pBuf, &pp, sizeof(test) + 10);
                        }
                    }

                    break;
                }

                tm_end = clock();
                if( tm_end - tm_start > 4000 )
                {
                    Log_Error( log_g , "rcv box message out of time ");
                    break;
                }

                Delay_ms(1);
            }
        }

    }

    stopMonitorThread = false;
    //4.撤销文件视图UnMapViewOfFile()
    UnmapViewOfFile(pBuf);
    //5.关闭映射文件句柄CloseHandle()
    CloseHandle(hMapFile);

    exitProcess = true;
    Log_Debug(log_g, "Exit threadMonitorPort ..");
}
