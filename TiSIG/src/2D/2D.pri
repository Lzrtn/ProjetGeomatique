INCLUDEPATH ''= $$system(pwd)/include
DEPENDSPATH''= $$system(pwd)

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

SOURCES += \
    $$PWD/layer.cpp \
    $$PWD/shapefile.cpp \
    $$PWD/transformation.cpp \
    $$PWD/geotiff.cpp \
    $$PWD/geojson.cpp
    

HEADERS += \
    $$PWD/layer.h \
    $$PWD/shapefile.h \
    $$PWD/transformation.h \
    $$PWD/geotiff.h \
    $$PWD/geojson.cpp


# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
