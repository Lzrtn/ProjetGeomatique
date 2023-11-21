#include "transformation.h"
#include <string>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>

Transformation::Transformation()
{
}

Transformation::~Transformation(){

}

QPolygonF Transformation::JSONtoCoords(std::string polygone)
{
    // Utilisez json::parse pour convertir la chaîne JSON en un objet JSON explicite
    nlohmann::json polygoneJSON = nlohmann::json::parse(polygone);

    // Accédez aux valeurs du JSON comme d'habitude
    std::string type = polygoneJSON["type"];
    //std::cout<<"type "<<type<<std::endl;
    //On sélectionne [] car nous avons un polygone, et pas un multi_polygone
    //std::cout<<"coordinates[i][j]"<<polygoneJSON["coordinates"][0]<<std::endl;

    QVector <QPointF> polygoneCoordinates;
    for (int i=0; i < polygoneJSON["coordinates"][0].size();i++)
    {
        double x = polygoneJSON["coordinates"][0][i][0];
        double y = polygoneJSON["coordinates"][0][i][1];
        polygoneCoordinates.push_back(QPointF(x, -y));
    }
    QPolygonF polygoneToPlot(polygoneCoordinates);

    return polygoneToPlot;

}
