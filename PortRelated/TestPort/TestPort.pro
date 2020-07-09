#-------------------------------------------------
#
# Project created by QtCreator 2020-05-07T19:20:40
#
#-------------------------------------------------
QT -= gui
QT += core

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TARGET = TestPort
TEMPLATE = app

#include(../global.pri)

#include
#INCLUDEPATH += $${SDK_PATH}/../include

#INCLUDEPATH += $${SDK_PATH}/log4cxx/include
INCLUDEPATH += E://svnProjects/publicSDK/log4cxx/include
LIBS += E://svnProjects/publicSDK/log4cxx/x64.VS2013/log4cxx.lib
#LIBS += G://working/QtProjects/publicSDK/log4cxx/Lib/log4cxx.lib

#INCLUDEPATH += G://working/QtProjects/publicSDK/QextSerialport
INCLUDEPATH += E://svnProjects/publicSDK/QextSerialport/include
LIBS += E://svnProjects/publicSDK/QextSerialport/Lib/VS2013/qextserialport.lib
#LIBS += G://working/QtProjects/publicSDK/QextSerialport/Release/qextserialport.lib


SOURCES += main.cpp\
    E://svnProjects/publicSDK/QextSerialport/include/comport/comportsystem.cpp \
    #mainwindow.cpp \
    myserialport.cpp

HEADERS  += \
    E://svnProjects/publicSDK/QextSerialport/include/comport/comportsystem.h \
    #mainwindow.h \
    myserialport.h

#FORMS    += mainwindow.ui
