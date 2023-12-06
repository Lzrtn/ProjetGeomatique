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

#For Curl
QT += network 
# Config for Curl
LIBS += -lcurl

# Config for opencv
CONFIG += link_pkgconfig
PKGCONFIG += opencv

SOURCES += \
    $$PWD/layer.cpp \
    $$PWD/shapefile.cpp \
    $$PWD/transformation.cpp \
    $$PWD/geotiff.cpp \
    $$PWD/geojson.cpp \
    $$PWD/wms.cpp \
    $$PWD/wmts.cpp


    

HEADERS += \
    $$PWD/layer.h \
    $$PWD/shapefile.h \
    $$PWD/transformation.h \
    $$PWD/geotiff.h \
    $$PWD/geojson.h \
    $$PWD/wms.h \
    $$PWD/wmts.h


# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17


