#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T16:19:23
#
#-------------------------------------------------

QT       += core

QT       += gui widgets
QT       += websockets
QT       += network
QT       += sql
TARGET = jsonserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    database.cpp \
    mainwindow2.cpp \
    sendthread.cpp \
    serversocket.cpp

HEADERS += \
    mainwindow.h \
    database.h \
    mainwindow2.h \
    sendthread.h \
    serversocket.h

FORMS += \
    mainwindow.ui \
    mainwindow2.ui

RESOURCES += \
    files.qrc

RC_FILE = jsonserver.rc
