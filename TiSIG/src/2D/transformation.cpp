#include "transformation.h"
#include "../data/json.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <QLineF>

// Constructor
Transformation::Transformation()
{
}

// Destructor
Transformation::~Transformation(){

}

std::string Transformation::whichCRS(std::string data)
{
    nlohmann::json dataJSON = nlohmann::json::parse(data); // Parsing
    return dataJSON["crs"]["properties"]["name"]; // Get the EPSG

}

std::string Transformation::whatType(std::string data)
{
    nlohmann::json dataJSON = nlohmann::json::parse(data); // Parsing
    std::string type = dataJSON["type"];
    return type;
}

QPolygonF Transformation::JSONtoCoordsPOL(std::string polygone)
{
    // Convert the polygone std::string to a json format
    nlohmann::json polygoneJSON = nlohmann::json::parse(polygone); // Parsing
    std::string type = polygoneJSON["type"];
    QVector <QPointF> polygoneCoordinates; // Creation of a polygon instance
    QVector <QPointF> polygoneCoordinatesToSubstract; // Creation of a polygon instance (case of a "donut" polygon)
    // Get coordinates of the first polygon
    for (int i=0; i < (int)polygoneJSON["coordinates"][0].size();i++)
    {
        double x = polygoneJSON["coordinates"][0][i][0];
        double y = polygoneJSON["coordinates"][0][i][1];
        polygoneCoordinates.push_back(QPointF(x, -y));
    }
    // Get coordinats of the polygon to substract, if there is one
    for (int i=0; i < (int)polygoneJSON["coordinates"][1].size();i++)
    {
        double xSubstr = polygoneJSON["coordinates"][1][i][0];
        double ySubstr = polygoneJSON["coordinates"][1][i][1];
        polygoneCoordinatesToSubstract.push_back(QPointF(xSubstr, -ySubstr));
    }
    QPolygonF polygoneFull(polygoneCoordinates);

    QPolygonF polygoneToSubstract(polygoneCoordinatesToSubstract);

    QPolygonF polygoneToPlot = polygoneFull.subtracted(polygoneToSubstract); // Let's make the hole in the polygon, if there is one

    return polygoneToPlot;

}

std::vector<QVector <QLineF>> Transformation::JSONtoCoordsLIN(std::string line)
{
    std::vector<QVector <QLineF>> liste_de_polyligne;
    nlohmann::json lineJSON = nlohmann::json::parse(line); // PArsing
    std::string type = lineJSON["type"];
    if (type=="MultiLineString"){ // Case of a MultiLineString
        for (int i=0; i < (int)lineJSON["coordinates"].size();i++)
        {

            if(lineJSON["coordinates"][i].size()>3){
                QVector <QLineF> lineCoordinates;
                // Loop that construct every segment of the linestring
                for (int j =1; j<(int)lineJSON["coordinates"][i].size(); j++)
                {
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
    }
    else if (type == "LineString"){ // Case of the LineString
        QVector <QLineF> lineCoordinates;
        // Loop that construct the LineString
        for (int j =1; j<(int)lineJSON["coordinates"].size(); j++)
        {
            double x = lineJSON["coordinates"][j][0];
            double y = lineJSON["coordinates"][j][1];
            double xprec = lineJSON["coordinates"][j-1][0];
            double yprec = lineJSON["coordinates"][j-1][1];
            QLineF segmentToPlot(QPointF(xprec, -yprec),QPointF(x, -y)); // Link every segment to the previous one
            lineCoordinates.push_back(segmentToPlot);
        }
        liste_de_polyligne.push_back(lineCoordinates);
    }
    return liste_de_polyligne;
}


std::vector <QPointF> Transformation::JSONtoCoordsPTS(std::string point)
{
    std::vector<QPointF> liste_de_points;

    nlohmann::json pointJSON = nlohmann::json::parse(point); // Parsing
    double x = pointJSON["coordinates"][0];
    double y = pointJSON["coordinates"][1];
    liste_de_points.push_back(QPointF(x,-y)); // Create a list that contains every point of the shapefile
    return liste_de_points;
}
