#-------------------------------------------------
#
# Project created by QtCreator 2013-06-23T16:32:48
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IntelligentCameraSystem
TEMPLATE = app


#Linux includes
linux:INCLUDEPATH += "/home/dustin/Downloads/boost_1_53_0/"
linux:include("/home/dustin/Downloads/QsLog/QsLog.pri")

#windows includes
win32:INCLUDEPATH += "C:\tools\vlc-2.0.7\vlc-2.0.7\include"
win32:include("C:\Users\Dustin\Downloads\QsLog\QsLog.pri")

SOURCES += main.cpp \
    mainwindow.cpp \
    Util/packetsender.cpp \
    Network/address.cpp \
    Network/socket.cpp \
    UI/streamertextedit.cpp \
    UI/vlcvideowidget.cpp \
    Util/packetreceiver.cpp \
    Util/messagedispatcher.cpp \
    Video/vlcwrapper.cpp \
    Video/vlctoqtinterface.cpp \
    Network/tcpclient.cpp \
    Network/networktoqtinterface.cpp \
    Util/tickingtimer.cpp \
    UI/serverpropertiesdialog.cpp \
    ThirdParty/pugixml/pugixml.cpp \
    Util/FileUtil/serverconfigfileutil.cpp \
    logindialog.cpp \
    UI/videowindow.cpp \
    UI/videocontrolswidget.cpp \
    controlcenter.cpp \
    Types/camera.cpp \
    Video/ffmpegwrapper.cpp \
    Video/demuxer.cpp \
    UI/ffmpegvideowidget.cpp \
    Util/FileUtil/filereaderutility.cpp \
    Types/mediastatistics.cpp \
    Util/FileUtil/bandwidthfilereader.cpp \
    Util/FileUtil/cameralistfileutil.cpp \
    DecisionModule/decisioninterface.cpp \
    Types/encodingparameters.cpp \
    Types/featureset.cpp

HEADERS  += \
    mainwindow.h \
    Util/packetsender.h \
    Network/address.h \
    Network/socket.h \
    Global/SocketTypes.h \
    Global/SocketConstants.h \
    UI/streamertextedit.h \
    Global/StdTypes.h \
    UI/vlcvideowidget.h \
    Global/Log.h \
    Util/packetreceiver.h \
    Util/messagedispatcher.h \
    Video/vlcwrapper.h \
    Video/vlctoqtinterface.h \
    Network/tcpclient.h \
    Network/networktoqtinterface.h \
    Util/tickingtimer.h \
    UI/serverpropertiesdialog.h \
    ThirdParty/pugixml/pugixml.hpp \
    ThirdParty/pugixml/pugiconfig.hpp \
    Util/FileUtil/serverconfigfileutil.h \
    logindialog.h \
    UI/videowindow.h \
    UI/videocontrolswidget.h \
    controlcenter.h \
    Types/camera.h \
    Video/ffmpegwrapper.h \
    Video/demuxer.h \
    UI/ffmpegvideowidget.h \
    Util/FileUtil/filereaderutility.h \
    Types/mediastatistics.h \
    Util/FileUtil/bandwidthfilereader.h \
    Global/Constants.h \
    Util/FileUtil/cameralistfileutil.h \
    DecisionModule/decisioninterface.h \
    Types/encodingparameters.h \
    Types/featureset.h

FORMS    += \
    mainwindow.ui \
    UI/serverpropertiesdialog.ui \
    logindialog.ui \
    UI/videowindow.ui \
    controlcenter.ui


#Linux libraries
linux:LIBS += -L/usr/local/lib/
linux:LIBS += -lglog
linux:LIBS += -L/usr/lib/
linux:LIBS += -lvlc
linux:LIBS += -lavformat -lavcodec -lavutil -lswscale


#windows libraries
win32:LIBS += -lwsock32 -lws2_32
win32:LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC" -llibvlc

RESOURCES +=
