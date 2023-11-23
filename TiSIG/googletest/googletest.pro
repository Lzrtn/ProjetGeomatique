QT +=testlib
CONFIG += testlib

SOURCES += \
        google.cpp \
        test_camera.cpp
        bddgoogle.cpp \
        executorGoogle.cpp \
        $$PWD/../src/outils/dbmanager.cpp \
        $$PWD/../src/outils/docker.cpp \
        $$PWD/../src/outils/executor.cpp \
        main.cpp
#        bddgoogle.cpp \
#        executorGoogle.cpp \


# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target


# Utilisez le chemin correct pour les bibliothèques et les en-têtes installés par Google Test
INCLUDEPATH += /usr/include/gtest
LIBS += -lgtest -lgtest_main
LIBS += -lpqxx


testlib.commands = ./googletest
