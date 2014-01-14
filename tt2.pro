#-------------------------------------------------
#
# Project created by QtCreator 2014-01-07T15:39:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tt2
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp

HEADERS  += window.h

INCLUDEPATH += /usr/include/qwt
LIBS += -L/usr/lib -lqwt
