#-------------------------------------------------
#
# Project created by QtCreator 2011-01-30T17:34:09
#
#-------------------------------------------------

QT       += core gui network
QT       +=widgets

CONFIG += static

win32:LIBS += -LC:\WpdPack\Lib -lwpcap
win32:INCLUDEPATH += C:\WpdPack\include

TARGET = QtNetEmu
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    networkinterface.cpp \
    reader_thread.cpp \
    buffer.cpp \
    writer_thread.cpp \
    PerformanceTimers.cpp \
    monitorthread.cpp

HEADERS  += mainwindow.h \
    networkinterface.h \
    reader_thread.h \
    buffer.h \
    writer_thread.h \
    PerformanceTimers.h \
    monitorthread.h \
    CollectedStatistics.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    test.txt \
    buffer.txt
