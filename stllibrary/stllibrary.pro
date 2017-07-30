#-------------------------------------------------
#
# Project created by QtCreator 2017-07-30T12:57:07
#
#-------------------------------------------------

QT += core gui opengl widgets
LIBS += -lOpengl32
TEMPLATE = lib

DEFINES += STLLIBRARY_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) {
    TARGET = fstld
} else {
    TARGET = fstl
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

SOURCES += src/backdrop.cpp \
    src/canvas.cpp \
    src/glmesh.cpp \
    src/loader.cpp \
    src/mesh.cpp \
    src/stlwindow.cpp

HEADERS += stllibrary_global.h \
    src/backdrop.h \
    src/canvas.h \
    src/glmesh.h \
    src/loader.h \
    src/mesh.h \
    src/vertex.h \
    src/stlwindow.h

DISTFILES += \
    exe/package.sh \
    exe/fstl.ico \
    exe/fstl.rc \
    gl/sphere.stl \
    gl/mesh.frag \
    gl/quad.frag \
    gl/mesh.vert \
    gl/quad.vert \
    src/style.qss

RESOURCES += \
    gl/gl.qrc \
    src/qt.qrc
