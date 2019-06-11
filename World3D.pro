# -------------------------------------------------
# Project created by QtCreator 2010-08-07T09:01:03
# -------------------------------------------------
TARGET = world3d
TEMPLATE = app
RC_FILE = world3d.rc
RESOURCES = world3d.qrc
SOURCES += main.cpp \
    mainwindow.cpp \
    qt_videodev.cpp \
    math3d/videodev.cpp \
    math3d/obshapes.cpp \
    math3d/object3D.cpp \
    math3d/obimages.cpp \
    math3d/obfrsh.cpp \
    math3d/eventcon.cpp \
    math3d/World3D.cpp \
    math3d/analytic.cpp \
    math3d/far_object.cpp
HEADERS += mainwindow.h \
    qt_videodev.h \
    math3d/eventcon.h \
    math3d/videodev.h \
    math3d/obshapes.h \
    math3d/object3D.h \
    math3d/obimages.h \
    math3d/obfrsh.h \
    math3d/eventcon.h \
    math3d/World3D.h \
    math3d/analytic.h \
    math3d/far_object.h \
    physics.h \
    astronomy.h \
    solar.h \
    tech.h \
    pshapes.h
FORMS += mainwindow.ui
QT += widgets
