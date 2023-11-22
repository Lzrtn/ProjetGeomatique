QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    building3d.cpp \
    camera.cpp \
    cameracontrols.cpp \
    dataflowwindow.cpp \
    helpwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    openglcityview.cpp

HEADERS += \
    camera.h \
    cameracontrols.h \
    dataflowwindow.h \
    helpwindow.h \
    building3d.h \
    mainwindow.h \
    openglcityview.h

FORMS += \
    dataflowwindow.ui \
    helpwindow.ui \
    mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target