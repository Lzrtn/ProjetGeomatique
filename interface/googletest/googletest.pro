QT +=testlib
CONFIG += testlib

SOURCES += \
        google.cpp \
        main.cpp \
        test_camera.cpp


# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target


# Utilisez le chemin correct pour les bibliothèques et les en-têtes installés par Google Test
INCLUDEPATH += /usr/include/gtest
LIBS += -lgtest -lgtest_main


testlib.commands = ./googletest
