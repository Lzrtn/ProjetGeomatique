QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/dataflowwindow.cpp \
    $$PWD/helpwindow.cpp \
#    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp

HEADERS += \
    $$PWD/dataflowwindow.h \
    $$PWD/helpwindow.h \
    $$PWD/mainwindow.h

FORMS += \
    $$PWD/dataflowwindow.ui \
    $$PWD/helpwindow.ui \
    $$PWD/mainwindow.ui
