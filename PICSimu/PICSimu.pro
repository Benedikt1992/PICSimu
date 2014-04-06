#-------------------------------------------------
#
# Project created by QtCreator 2014-04-05T09:51:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PICSimu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    codeline.cpp \
    parser.cpp \
    prozessor.cpp \
    speicher.cpp \
    steuerwerk.cpp \
    selectfiledialog.cpp

HEADERS  += mainwindow.h \
    codeline.h \
    parser.h \
    prozessor.h \
    speicher.h \
    steuerwerk.h \
    selectfiledialog.h

FORMS    += mainwindow.ui \
    selectfiledialog.ui
