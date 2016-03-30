#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T11:44:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = master
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    emitter.cpp \
    object.cpp \
    receiver.cpp \
    obstacle.cpp \
    paintwidget.cpp \
    simulation.cpp \
    console/console.cpp \
    console/settingsdialog.cpp

HEADERS  += mainwindow.h \
    emitter.h \
    object.h \
    receiver.h \
    obstacle.h \
    paintwidget.h \
    constants.h \
    simulation.h \
    console/console.h \
    console/settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
CONFIG += c++11
