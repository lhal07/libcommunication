#-------------------------------------------------
#
# Project created by Luis Lourenco, February,2016
#
#-------------------------------------------------

QT       += core
QT       += network
#QT       += concurrent #for Qt5
QT       -= gui

TARGET  = communication
DEFINES += "VERSION=\"$$cat(VERSION)\""
CONFIG += shared
CONFIG += c++
CONFIG -= app_bundle
CONFIG -= lib_bundle

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = lib

INCLUDEPATH += /usr/include/Log

LIBS += -lboost_system\
        -lboost_filesystem\
        -lboost_thread\
        -lnetconfig\
        -lsimplesocket++\
        -lcurl\
        -lcurlpp\
        -llog

HEADERS += Connection.h\
           SocketConnection.h\
           Rest.h\
           Config.h

SOURCES += Connection.cpp\
           SocketConnection.cpp\
           Rest.cpp\
           Config.cpp
