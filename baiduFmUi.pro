QT       += core gui network multimedia
#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QBaiduFm
TEMPLATE = app

CONFIG += c++11

include(./qhotkey.pri)

SOURCES += main.cpp\
        widget.cpp \
    aristpicture.cpp \
    lyrics.cpp \
    channelswidget.cpp \
    logindlg.cpp \
    volpannel.cpp \
    getbaiduchannel.cpp \
    getsongplaylistid.cpp \
    getsongreallink.cpp \
    getaristpic.cpp \
    getlrc.cpp \
    getcirclescaledpixmap.cpp \
    eventlabel.cpp \
    downloadfile.cpp \
    appsearchline.cpp \
    titlewidget.cpp


HEADERS  += widget.h \
    aristpicture.h \
    lyrics.h \
    channelswidget.h \
    logindlg.h \
    volpannel.h \
    getbaiduchannel.h \
    getsongplaylistid.h \
    getsongreallink.h \
    getaristpic.h \
    getlrc.h \
    getcirclescaledpixmap.h \
    eventlabel.h \
    downloadfile.h \
    appsearchline.h \
    titlewidget.h


FORMS    += widget.ui \
    aristpicture.ui \
    lyrics.ui \
    channelswidget.ui \
    logindlg.ui

RESOURCES += \
    img.qrc \
