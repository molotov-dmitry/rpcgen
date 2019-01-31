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
    generatorserversource.cpp \
    generatorheader.cpp \
    generatorclientsource.cpp \
    generatorstub.cpp

HEADERS += \
    settings.h \
    parser.h \
    utils.h \
    generator.h \
    generatorserversource.h \
    generatorheader.h \
    generatorclientsource.h \
    generatorstub.h
