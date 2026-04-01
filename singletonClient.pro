QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    singletonclient.cpp \
    singleton_main.cpp

HEADERS += \
    singletonclient.h