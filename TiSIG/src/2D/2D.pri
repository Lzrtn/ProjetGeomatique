INCLUDEPATH ''= $$system(pwd)/include
DEPENDSPATH''= $$system(pwd)

QT       += core gui
QT += network core

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

# Indiquez l'emplacement de QuaZip
INCLUDEPATH += /usr/include/quazip5
LIBS += -lquazip5

INCLUDEPATH += /usr/include/proj
LIBS += -lproj


SOURCES += \
    $$PWD/layer.cpp \
    $$PWD/shapefile.cpp \
    $$PWD/transformation.cpp \
    $$PWD/geotiff.cpp \
    $$PWD/geojson.cpp \
    $$PWD/rasteritem.cpp \
    $$PWD/rasterimport.cpp \
    $$PWD/geotiff_to_obj.cpp \
    $$PWD/wfsflow.cpp \
#    $$PWD/wmsflow.cpp \
#    $$PWD/wmtsflow.cpp \
    $$PWD/crs_converter.hpp \
    $$PWD/shpwfsflow.cpp \

HEADERS += \
    $$PWD/layer.h \
    $$PWD/shapefile.h \
    $$PWD/transformation.h \
    $$PWD/geotiff.h \
    $$PWD/geojson.h \
    $$PWD/rasteritem.h \
    $$PWD/rasterimport.h \
    $$PWD/geotiff_to_obj.h \
    $$PWD/wfsflow.h \
#    $$PWD/wmsflow.h \
#    $$PWD/wmtsflow.h \
    $$PWD/shpwfsflow.h \

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17
