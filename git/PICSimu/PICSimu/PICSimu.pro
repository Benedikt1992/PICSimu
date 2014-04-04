#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T22:26:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = PICSimu
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    codeline.cpp \
    steuerwerk.cpp \
    prozessor.cpp \
    speicher.cpp \
    parser.cpp

HEADERS += \
    codeline.h \
    steuerwerk.h \
    prozessor.h \
    speicher.h \
    parser.h
