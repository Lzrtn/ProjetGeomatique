QT +=testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include (../src/2D/2D.pri)
include (../src/outils/outils.pri)

SOURCES += \
        main.cpp


# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
