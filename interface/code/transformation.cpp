#include "transformation.h"
#include <string>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <QLineF>

Transformation::Transformation()
{
}

Transformation::~Transformation(){

}

QPolygonF Transformation::JSONtoCoordsPOL(std::string polygone)
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

std::vector<QVector <QLineF>> Transformation::JSONtoCoordsLIN(std::string line)
{
    std::vector<QVector <QLineF>> liste_de_polyligne;
    // Utilisez json::parse pour convertir la chaîne JSON en un objet JSON explicite
    nlohmann::json lineJSON = nlohmann::json::parse(line);
    std::string type = lineJSON["type"];
    for (int i=0; i < lineJSON["coordinates"].size();i++)
    {

        if(lineJSON["coordinates"][i].size()>3){
            QVector <QLineF> lineCoordinates;
            //std::cout<<"premiere boucle, la polyligne est : "<<lineJSON["coordinates"][i]<<std::endl;
            for (int j =1; j<lineJSON["coordinates"][i].size(); j++)
            {
                //std::cout<<"deuxième boucle, le node est : "<<lineJSON["coordinates"][i][j]<<std::endl;
                double x = lineJSON["coordinates"][i][j][0];
                double y = lineJSON["coordinates"][i][j][1];
                double xprec = lineJSON["coordinates"][i][j-1][0];
                double yprec = lineJSON["coordinates"][i][j-1][1];
                QLineF segmentToPlot(QPointF(xprec, yprec),QPointF(x, y));
                lineCoordinates.push_back(segmentToPlot);
            }
            liste_de_polyligne.push_back(lineCoordinates);
        }
    }
    return liste_de_polyligne;
}
