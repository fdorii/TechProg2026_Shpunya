QMAKE_LFLAGS += -F/opt/homebrew/lib
<<<<<<< Updated upstream

QT += core network widgets gui
=======
>>>>>>> Stashed changes

QT += core gui widgets network

CONFIG += c++17
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
<<<<<<< Updated upstream
    task1.cpp
=======
    task2form.cpp \
    task1.cpp \
    task3.cpp
>>>>>>> Stashed changes

HEADERS += \
    clientfunctions.h \
    loginform.h \
    mainwindow.h \
    registrationform.h \
    singletonclient.h \
    statisticwindow.h \
<<<<<<< Updated upstream
    task1.h \
    ui_task1.h
=======
    task2form.h \
    task1.h \
    task3.h
>>>>>>> Stashed changes

FORMS += \
    loginform.ui \
    mainwindow.ui \
    registrationform.ui \
    statisticwindow.ui \
<<<<<<< Updated upstream
    task1.ui
=======
    task2form.ui \
    task1.ui \
    task3.ui
>>>>>>> Stashed changes
