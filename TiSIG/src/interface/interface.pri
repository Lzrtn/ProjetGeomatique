QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/layermanager3d.cpp \
    $$PWD/mntwindow.cpp \
    $$PWD/helpwindow.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/view_zoom.cpp \
    $$PWD/wfsdataflowwindow.cpp \
    $$PWD/wmsdataflowwindow.cpp

HEADERS += \
    $$PWD/layermanager3d.h \
    $$PWD/mntwindow.h \
    $$PWD/helpwindow.h \
    $$PWD/mainwindow.h \
    $$PWD/view_zoom.h \
    $$PWD/wfsdataflowwindow.h \
    $$PWD/wmsdataflowwindow.h

FORMS += \
    $$PWD/mntwindow.ui \
    $$PWD/helpwindow.ui \
    $$PWD/mainwindow.ui \
    $$PWD/wfsdataflowwindow.ui \
    $$PWD/wmsdataflowwindow.ui
