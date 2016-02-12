#-------------------------------------------------
#
# Project created by Luis Lourenco, February,2016
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET  = communication
VERSION = 0.1-0
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
        -llog

HEADERS += Command.h\
           Connection.h\
           Config.h

SOURCES += Command.cpp\
           Connection.cpp\
           Config.cpp
