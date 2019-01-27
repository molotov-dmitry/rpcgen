TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    settings.cpp \
    parser.cpp \
    utils.cpp

HEADERS += \
    settings.h \
    parser.h \
    utils.h
