QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

include($$PWD/3D/3D.pri)
include($$PWD/2D/2D.pri)
include($$PWD/interface/interface.pri)
include($$PWD/outils/outils.pri)

SOURCES += $$PWD/main.cpp

# Copy icons and data
copydata.commands = $(COPY_DIR) $$PWD/icons $$OUT_PWD && $(COPY_DIR) $$PWD/data $$OUT_PWD ;
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


