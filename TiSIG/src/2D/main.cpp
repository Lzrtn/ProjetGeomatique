#include "2D/mainwindow.h"
#include <QApplication>
#include <string>
#include "2D/transformation.h"
#include <pqxx/pqxx>
#include <iostream>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <string>
#include <algorithm>
#include "outils/dbmanager.h"
#include "outils/docker.h"

int main(int argc, char *argv[])

{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
