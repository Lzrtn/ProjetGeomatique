QT +=testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include ($$PWD/../src/2D/2D.pri)
include ($$PWD/../src/3D/3D.pri)
include ($$PWD/../src/outils/outils.pri)
include ($$PWD/../src/interface/interface.pri)

SOURCES += \
    $$PWD/main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
