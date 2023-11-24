QT +=testlib
CONFIG += testlib

SOURCES += \
        google.cpp \
	test_camera.cpp \
	bddgoogle.cpp \
	executorGoogle.cpp \
	test-geotiff.cpp
	
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


testlib.commands = ./googletest
