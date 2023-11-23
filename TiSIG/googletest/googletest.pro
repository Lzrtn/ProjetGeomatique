QT +=testlib
CONFIG += testlib

SOURCES += \
        bddgoogle.cpp \
        executorGoogle.cpp \
        google.cpp \
        main.cpp \
        $$PWD/../src/outils/dbmanager.cpp \
        $$PWD/../src/outils/docker.cpp \
        $$PWD/../src/outils/executor.cpp \



# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target


# Utilisez le chemin correct pour les bibliothèques et les en-têtes installés par Google Test
INCLUDEPATH += /usr/include/gtest
LIBS += -lgtest -lgtest_main


testlib.commands = ./googletest
