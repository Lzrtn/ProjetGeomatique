QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/compass.cpp \
    $$PWD/object3d.cpp \
    3D/building3d.cpp \
    3D/openglcityview.cpp \
    3D/camera.cpp \
    3D/cameracontrols.cpp

HEADERS += \
    $$PWD/compass.h \
    $$PWD/object3d.h \
    3D/building3d.h \
    3D/openglcityview.h \
    3D/camera.h \
    3D/cameracontrols.h

RESOURCES += \
    3D/shaders.qrc \
    3D/textures.qrc \
    3D/objects.qrc
