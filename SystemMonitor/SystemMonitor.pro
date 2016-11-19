#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T17:09:55
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SystemMonitor
TEMPLATE = app

SOURCES += main.cpp \
           system_monitor.cpp \
           qcustomplot.cpp \
           info_battery.cpp \
           info_cpu.cpp \
           info_memory.cpp \
           info_process.cpp

HEADERS  += system_monitor.h \
            qcustomplot.h \
            info_battery.h \
            info_cpu.h \
            info_memory.h \
            info_process.h

FORMS    += system_monitor.ui

CONFIG   += c++11
