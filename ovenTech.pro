#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T15:09:26
#
#-------------------------------------------------

QT       += core gui sql printsupport

CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ovenTech
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(xlsx/qtxlsx.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    db/dbxlsx.cpp \
    modelproc.cpp \
    cfgplot.cpp \
    dialogprog.cpp \
    srcplot.cpp \
    plotdata.cpp \
    modelalarm.cpp \
    olap/axiswidget.cpp \
    olap/cubewidget.cpp \
    olap/cubic.cpp \
    olap/olapmodel.cpp \
    olap/tableview.cpp

HEADERS += \
        mainwindow.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    db/dbxlsx.h \
    modelproc.h \
    cfgplot.h \
    dialogprog.h \
    srcplot.h \
    plotdata.h \
    modelalarm.h \
    olap/axiswidget.h \
    olap/cubewidget.h \
    olap/cubic.h \
    olap/olapmodel.h \
    olap/tableview.h

FORMS += \
        mainwindow.ui \
    db/dblogin.ui \
    cfgplot.ui \
    dialogprog.ui \
    olap/axiswidget.ui \
    olap/cubewidget.ui

LIBS     += -lqwt-qt5

RESOURCES += \
    res.qrc

DISTFILES +=

