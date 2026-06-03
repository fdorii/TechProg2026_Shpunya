QT -= gui
QT += network
QT += core widgets gui

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
    task2form.cpp\
    task1.cpp\
    task3.cpp

HEADERS += \
    clientfunctions.h \
    loginform.h \
    mainwindow.h \
    registrationform.h \
    singletonclient.h \
    statisticwindow.h \
    task2form.h\
    task1.h\
    task3.h

FORMS += \
    loginform.ui \
    mainwindow.ui \
    registrationform.ui \
    task2form.ui\
    task1.ui\
    task3.ui\
    statisticwindow.ui
