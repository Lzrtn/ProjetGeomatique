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


std::string Transformation::whatType(std::string data)
{
    nlohmann::json dataJSON = nlohmann::json::parse(data);
    std::string type = dataJSON["type"];
    std::cout<<type<<std::endl;
    return type;
}

QPolygonF Transformation::JSONtoCoordsPOL(std::string polygone)
{
    // Utilisez json::parse pour convertir la chaîne JSON en un objet JSON explicite
    nlohmann::json polygoneJSON = nlohmann::json::parse(polygone);
    std::string type = polygoneJSON["type"];
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
                QLineF segmentToPlot(QPointF(xprec, -yprec),QPointF(x, -y));
                lineCoordinates.push_back(segmentToPlot);
            }
            liste_de_polyligne.push_back(lineCoordinates);
        }
    }
    return liste_de_polyligne;
}


std::vector <QPointF> Transformation::JSONtoCoordsPTS(std::string point)
{
    std::vector<QPointF> liste_de_points;
    nlohmann::json pointJSON = nlohmann::json::parse(point);
    for (int i = 0; i< pointJSON["coordinates"].size(); i++)
    {
        double x = pointJSON["coordinates"][i][0];
        double y = pointJSON["coordinates"][i][1];
        liste_de_points.push_back(QPointF(x,y));
    }
    return liste_de_points;
}
