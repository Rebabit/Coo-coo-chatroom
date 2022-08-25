QT       += core gui network sql script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += resources_big
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Session/abstractsession.cpp \
    Session/addfriendrequestpop.cpp \
    Session/offlinesession.cpp \
    Session/onlinesession.cpp \
    UI_Tool/friendnotice.cpp \
    UI_Tool/mylable.cpp \
    clientdatacenter.cpp \
    clientmain.cpp \
    codeeditor.cpp \
    databaseoperation.cpp \
    kuang.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    messagemodel.cpp \
    myhighlighter.cpp \
    userlogin.cpp \
    usermodel.cpp \
    userregister.cpp \
    widget.cpp \
    emoji.cpp

HEADERS += \
    Session/abstractsession.h \
    Session/addfriendrequestpop.h \
    Session/offlinesession.h \
    Session/onlinesession.h \
    UI_Tool/friendnotice.h \
    UI_Tool/mylable.h \
    clientdatacenter.h \
    clientmain.h \
    codeeditor.h \
    databaseoperation.h \
    myhighlighter.h \
    typedef.h \
    kuang.h \
    ltest.h \
    mainwindow.h \
    message.h \
    messagemodel.h \
    userlogin.h \
    usermodel.h \
    userregister.h \
    widget.h \
    emoji.h

FORMS += \
    Session/addfriendrequestpop.ui \
    UI_Tool/friendnotice.ui \
    kuang.ui \
    mainwindow.ui \
    message.ui \
    userlogin.ui \
    userregister.ui \
    widget.ui \
    emoji.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
