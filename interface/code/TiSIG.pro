QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Ajouter les chemins des en-têtes et des bibliothèques GDAL
INCLUDEPATH += /chemin/vers/les/en-tetes/gdal
LIBS += -L/chemin/vers/les/bibliotheques/gdal -lgdal
LIBS += -lpq

INCLUDEPATH += /usr/include
LIBS += -L/usr/lib/x86_64-linux-gnu -lpqxx

SOURCES += \
    layer.cpp \
    main.cpp \
    mainwindow.cpp \
    shapefile.cpp \
    transformation.cpp
    shapefile.cpp

HEADERS += \
    layer.h \
    mainwindow.h \
    shapefile.h \
    transformation.h
    shapefile.h
    transformation.cpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

