QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/compass.cpp \
    $$PWD/object3d.cpp \
    $$PWD/building3d.cpp \
    $$PWD/openglcityview.cpp \
    $$PWD/camera.cpp \
    $$PWD/cameracontrols.cpp \
    $$PWD/batiment.cpp \ 
    $$PWD/batiments.cpp

HEADERS += \
    $$PWD/compass.h \
    $$PWD/object3d.h \
    $$PWD/building3d.h \
    $$PWD/openglcityview.h \
    $$PWD/camera.h \
    $$PWD/cameracontrols.h \
    $$PWD/batiment.h \ 
    $$PWD/batiments.h

RESOURCES += \
    $$PWD/shaders.qrc \
    $$PWD/textures.qrc \
    $$PWD/objects.qrc
