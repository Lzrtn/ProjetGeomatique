#include "mainwindow.h"
#include <QApplication>
#include <string>
#include "transformation.h"
#include <pqxx/pqxx>
#include <iostream>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>

int main(int argc, char *argv[])
{/*
    try {
        // Connect to the database
        pqxx::connection c("user=postgres password=postgres host=localhost port=5432 dbname=essai_dbf target_session_attrs=read-write");

        pqxx::work k(c);
        pqxx::result rows = k.exec("SELECT id, ST_AsGeoJSON(geom) FROM arrondissement;");

        // Check if there is at least one row in the result
        if (!rows.empty()) {
            const auto& row = rows[0]; // Access the first row

            // Access columns by index
            auto id = row[0].as<std::string>();
            auto geojson = row[1].as<std::string>();
            Transformation t;
            QPolygonF polygoneToPlot = t.JSONtoCoords(geojson);
        } else {
            std::cout << "No rows in the result." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
