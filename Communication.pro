#-------------------------------------------------
#
# Project created by Luis Lourenco, February,2016
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET  = communication
VERSION = 0.2-1
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

HEADERS += Command.h\
           Connection.h\
           Rest.h\
           Config.h

SOURCES += Command.cpp\
           Connection.cpp\
           Rest.cpp\
           Config.cpp
