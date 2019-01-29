TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

OBJECTS_DIR = obj

SOURCES += main.cpp \
    settings.cpp \
    parser.cpp \
    utils.cpp \
    generator.cpp \
    generatorserverheader.cpp \
    generatorserversource.cpp

HEADERS += \
    settings.h \
    parser.h \
    utils.h \
    generator.h \
    generatorserverheader.h \
    generatorserversource.h
