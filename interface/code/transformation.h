#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <string>
#include <nlohmann/json.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
class Transformation
{
public:
    Transformation();
    ~Transformation();
    QPolygonF JSONtoCoordsPOL(std::string polygone);
    std::vector<QVector <QLineF>> JSONtoCoordsLIN(std::string line);
};

#endif // TRANSFORMATION_H
