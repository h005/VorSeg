#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T08:52:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VorSeg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    labelview.cpp \
    beachline.cpp \
    dcel.cpp \
    edge.cpp \
    eventqueue.cpp \
    face.cpp \
    sitequeue.cpp \
    vertex.cpp \
    uf.cpp

HEADERS  += mainwindow.h \
    labelview.h \
    beachline.h \
    dcel.h \
    edge.h \
    eventqueue.h \
    face.h \
    sitequeue.h \
    vertex.h \
    uf.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:\opencv\include\
            D:\opencv\include\opencv\
            D:\opencv\include\opencv2

LIBS += D:/opencv/lib/*.dll.a
