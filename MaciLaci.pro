#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T22:46:50
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $$PWD #a relatív útvonalhoz kell

TARGET = MaciLaci
TEMPLATE = app


SOURCES += main.cpp\
        macilaciwidget.cpp \
    movingcharacters.cpp \
    ranger.cpp \
    gamecontroller.cpp \
    player.cpp \
    gameoverwidget.cpp

HEADERS  += macilaciwidget.h \
    gamecontroller.h \
    movingcharacters.h \
    ranger.h \
    player.h \
    soundsources.h \
    gameoverwidget.h

FORMS    += \
    gameoverwidget.ui

RESOURCES += \
    resources.qrc
