#-------------------------------------------------
#
# Project created by QtCreator 2014-09-08T18:27:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LAB1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    line.cpp \
    draw.cpp \
    shape.cpp \
    rectangle.cpp \
    ellipse.cpp

HEADERS  += mainwindow.h \
    draw.h \
    line.h \
    rectangle.h \
    shape.h \
    ellipse.h

FORMS    += mainwindow.ui