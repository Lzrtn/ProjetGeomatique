QT +=testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include ($$PWD/../src/2D/2D.pri)
include ($$PWD/../src/3D/3D.pri)
include ($$PWD/../src/outils/outils.pri)

SOURCES += \
    $$PWD/main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
