QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

include(3D/3D.pri)
include(2D/2D.pri)
include(interface/interface.pri)
include(outils/outils.pri)

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
