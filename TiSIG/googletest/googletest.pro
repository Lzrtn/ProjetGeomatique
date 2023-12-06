QT +=testlib
CONFIG += testlib

SOURCES += \
        google.cpp \
	test_camera.cpp \
	bddgoogle.cpp \
	executorGoogle.cpp \
	test-geotiff.cpp \
	test_geojson.cpp \
	test_importraster.cpp
	
	main.cpp
#        bddgoogle.cpp \
#        executorGoogle.cpp \
include(../src/2D/2D.pri)
include(../src/outils/outils.pri)

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target


# Utilisez le chemin correct pour les bibliothèques et les en-têtes installés par Google Test
INCLUDEPATH += /usr/include/gtest
LIBS += -lgtest -lgtest_main
LIBS += -lpqxx

#For Curl
QT += network 
# Config for Curl
LIBS += -lcurl

# Config for opencv
INCLUDEPATH += /usr/include/opencv4/
CONFIG += link_pkgconfig
PKGCONFIG += opencv4


testlib.commands = ./googletest
