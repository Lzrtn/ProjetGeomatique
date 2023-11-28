QT +=testlib
CONFIG += testlib

SOURCES += \
    $$PWD/google.cpp \
    $$PWD/test_camera.cpp \
    $$PWD/bddgoogle.cpp \
    $$PWD/executorGoogle.cpp \
    $$PWD/test-geotiff.cpp \
    $$PWD/test_geojson.cpp \
    $$PWD/main.cpp

include($$PWD/../src/2D/2D.pri)
include($$PWD/../src/3D/3D.pri)
include($$PWD/../src/outils/outils.pri)
#include($$PWD/../src/interface/interface.pri)

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target


# Utilisez le chemin correct pour les bibliothèques et les en-têtes installés par Google Test
INCLUDEPATH += /usr/include/gtest
LIBS += -lgtest -lgtest_main
LIBS += -lpqxx

testlib.commands = ./googletest

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
