QT +=testlib
CONFIG += testlib

SOURCES += \
        google.cpp \
	test_camera.cpp \
	bddgoogle.cpp \
	executorGoogle.cpp \
	test_importraster.cpp \
	$$PWD/../src/outils/dbmanager.cpp \
	$$PWD/../src/outils/docker.cpp \
	$$PWD/../src/outils/executor.cpp \
	$$PWD/../src/2D/wmts.cpp \
	$$PWD/../src/2D/wms.cpp \
	main.cpp
#        bddgoogle.cpp \
#        executorGoogle.cpp \
#		 test_importraster.cpp \


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
