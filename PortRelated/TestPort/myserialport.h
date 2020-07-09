#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H


#include <thread>
#include <windows.h>
#include "comport/comportsystem.h"
//#include "baseDefine.h"
#include "xLog.h"


//解决中文显示乱码问题
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

//共享内存
#define Delay_ms(x) Sleep(x)
#define BUF_SIZE 1024
const TCHAR cstrSzName[] = L"NameOfMappingObject";	//共享内存名
struct test
{
    int flag;   //0, 初值; 1, 读温度; 2, 退出
    //int flagWrite;  //0, 初值; 1, 温度值写入完成
    int value;
};

class MySerialPort : public ComPort
{
    Q_OBJECT
public:
    MySerialPort();

    void func1(std::string portName);
    void func2();

    bool stopMonitorThread; //停止测温线程
    bool exitProcess;

private:
    void threadMonitorPort();

private:

    test    pp;    //共享内存
    HANDLE  hMapFile;
    LPVOID  *pBuf;
};

#endif // MYSERIALPORT_H
