QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Ajouter les chemins des en-têtes et des bibliothèques GDAL
INCLUDEPATH += /usr/include/gdal
LIBS += -lgdal
LIBS += -lpq

INCLUDEPATH += /usr/include
LIBS += -L/usr/lib/x86_64-linux-gnu -lpqxx

include(../outils/outils.pri)

SOURCES += \
    building3d.cpp \
    dataflowwindow.cpp \
    helpwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    openglcityview.cpp \
    layer.cpp \
    shapefile.cpp \
    transformation.cpp

HEADERS += \
    dataflowwindow.h \
    helpwindow.h \
    building3d.h \
    mainwindow.h \
    openglcityview.h \
    layer.h \
    shapefile.h \
    transformation.h

FORMS += \
    dataflowwindow.ui \
    helpwindow.ui \
    mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc \

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
