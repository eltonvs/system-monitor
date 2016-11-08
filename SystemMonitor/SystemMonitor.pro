#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T17:09:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SystemMonitor
TEMPLATE = app

SOURCES += main.cpp \
           system_monitor.cpp \
           qcustomplot.cpp

HEADERS  += system_monitor.h \
            qcustomplot.h

FORMS    += system_monitor.ui
