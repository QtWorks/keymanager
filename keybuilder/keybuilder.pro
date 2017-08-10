#-------------------------------------------------
#
# Project created by QtCreator 2017-07-25T14:07:16
#
#-------------------------------------------------

QT       += core gui xml script
CONFIG += static
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += $$PWD/../stllibrary

TARGET = keybuilder
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

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../bin -lfstld
    TARGET = keybuilderd
} else {
    LIBS += -L$$PWD/../bin -lfstl
    TARGET = keybuilder
}

unix {
    DESTDIR = ../bin
    MOC_DIR = ../moc
    OBJECTS_DIR = ../obj
}

win32 {
    DESTDIR = ..\\bin
    MOC_DIR = ..\\moc
    OBJECTS_DIR = ..\\obj
}

unix {
    QMAKE_CLEAN *= $$DESTDIR/*$$TARGET*
    QMAKE_CLEAN *= $$MOC_DIR/*moc_*
    QMAKE_CLEAN *= $$OBJECTS_DIR/*.o*
}

win32 {
    QMAKE_CLEAN *= $$DESTDIR\\*$$TARGET*
    QMAKE_CLEAN *= $$MOC_DIR\\*moc_*
    QMAKE_CLEAN *= $$OBJECTS_DIR\\*.o*
}


SOURCES += main.cpp\
        mainwindow.cpp \
    collapsiblestack.cpp \
    parameter.cpp \
    cxmlnode.cpp \
    keybuilder.cpp \
    controller.cpp \
    parametermgr.cpp \
    layoutmgr.cpp \
    scriptmgr.cpp \
    captionlabel.cpp \
    collpasibleblock.cpp \
    parameterblock.cpp \
    exclusivechoicewidget.cpp \
    lineeditwidget.cpp \
    filepickerwidget.cpp \
    doubletripletwidget.cpp \
    genericparametertable.cpp \
    basewidget.cpp \
    dxforstlfilepicker.cpp \
    widgetfactory.cpp \
    yesnowidget.cpp \
    selectionmgr.cpp

HEADERS  += mainwindow.h \
    collapsiblestack.h \
    parameter.h \
    cxmlnode.h \
    keybuilder.h \
    controller.h \
    iservice.h \
    parametermgr.h \
    constants.h \
    layoutmgr.h \
    scriptmgr.h \
    captionlabel.h \
    collapsibleblock.h \
    parameterblock.h \
    exclusivechoicewidget.h \
    lineeditwidget.h \
    filepickerwidget.h \
    doubletripletwidget.h \
    genericparametertable.h \
    basewidget.h \
    dxforstlfilepicker.h \
    widgetfactory.h \
    yesnowidget.h \
    selectionmgr.h

FORMS    += mainwindow.ui \
    layoutmgr.ui \
    captionlabel.ui \
    parameterblock.ui \
    exclusivechoicewidget.ui \
    lineeditwidget.ui \
    filepickerwidget.ui \
    doubletripletwidget.ui \
    genericparametertable.ui \
    dxforstlfilepicker.ui \
    yesnowidget.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
