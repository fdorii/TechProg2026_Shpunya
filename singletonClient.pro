QMAKE_LFLAGS += -F/opt/homebrew/lib

QT += core network widgets gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
    client_main.cpp \
    clientfunctions.cpp \
    loginform.cpp \
    mainwindow.cpp \
    registrationform.cpp \
    singletonclient.cpp \
    statisticwindow.cpp \
    task1.cpp

HEADERS += \
    clientfunctions.h \
    loginform.h \
    mainwindow.h \
    registrationform.h \
    singletonclient.h \
    statisticwindow.h \
    task1.h \
    ui_task1.h

FORMS += \
    loginform.ui \
    mainwindow.ui \
    registrationform.ui \
    statisticwindow.ui \
    task1.ui
