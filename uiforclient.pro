QMAKE_LFLAGS += -F/opt/homebrew/lib

QT -= gui
QT += core widgets gui network

CONFIG += c++17 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ui_main.cpp \
    clientfunctions.cpp \
    loginform.cpp \
    mainwindow.cpp \
    registrationform.cpp \
    singletonclient.cpp

HEADERS += \
    clientfunctions.h \
    loginform.h \
    mainwindow.h \
    registrationform.h 

FORMS += \
    loginform.ui \
    mainwindow.ui \
    registrationform.ui
