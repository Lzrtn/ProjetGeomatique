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
    layer.cpp \
    main.cpp \
    mainwindow.cpp \
    shapefile.cpp \
    transformation.cpp\
    ../../test_db/dbmanager.cpp\
    ../../test_db/executor.cpp\
    ../../test_db/docker.cpp


HEADERS += \
    layer.h \
    mainwindow.h \
    shapefile.h \
    transformation.h\
    ../../test_db/dbmanager.h\
    ../../test_db/executor.h\
    ../../test_db/docker.h

FORMS += \
    mainwindow.ui

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

