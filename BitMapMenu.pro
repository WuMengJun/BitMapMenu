#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T14:29:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WXJ-Menu
TEMPLATE = app
CONFIG +=c++11
RC_ICONS = wxj.ico
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    toolsrc.qrc
