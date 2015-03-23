#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T16:43:35
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = uctoAuto
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../common/uctodocument.cpp \
    ../common/billingperiod.cpp \
    ../common/uctohelper.cpp

HEADERS += \
    ../common/uctodocument.h \
    ../common/billingperiod.h \
    ../common/uctohelper.h
