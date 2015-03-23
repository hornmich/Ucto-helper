#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T14:27:30
#
#-------------------------------------------------

QT       += core gui webkit printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = UctoHelper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../common/billingperiod.cpp \
    ../common/uctodocument.cpp \
    ../common/uctohelper.cpp

HEADERS  += mainwindow.h \
    ../common/billingperiod.h \
    ../common/uctodocument.h \
    ../common/uctohelper.h

FORMS    += mainwindow.ui
