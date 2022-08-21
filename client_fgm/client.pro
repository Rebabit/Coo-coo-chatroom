#-------------------------------------------------
#
# Project created by QtCreator 2022-08-19T16:57:37
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        login.cpp \
    clientsir.cpp \
    userregister.cpp \
    mainwindow.cpp \
    msgsession.cpp \
    clientdatacenter.cpp \
    Session/abstractsession.cpp \
    Session/offlinesession.cpp \
    Session/onlinesession.cpp \
    usermodel.cpp \
    messagemodel.cpp \
    databaseoperation.cpp \
    message.cpp

HEADERS  += login.h \
    clientsir.h \
    userregister.h \
    mainwindow.h \
    msgsession.h \
    clientdatacenter.h \
    Session/abstractsession.h \
    Session/offlinesession.h \
    Session/onlinesession.h \
    usermodel.h \
    messagemodel.h \
    databaseoperation.h \
    message.h

FORMS    += login.ui \
    userregister.ui \
    mainwindow.ui \
    msgsession.ui \
    message.ui

RESOURCES += \
    res.qrc
