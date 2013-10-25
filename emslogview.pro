#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T13:25:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = emslogview
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    egraph.cpp \
    qgraph.cpp \
    logloader.cpp \
    datafield.cpp \
    datapacketdecoder.cpp \
    dataselectionscreen.cpp

HEADERS  += mainwindow.h \
    egraph.h \
    qgraph.h \
    logloader.h \
    datafield.h \
    datapacketdecoder.h \
    dataselectionscreen.h

FORMS    += mainwindow.ui \
    dataselectionscreen.ui
