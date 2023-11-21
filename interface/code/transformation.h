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
    QPolygonF JSONtoCoords(std::string polygone);
};

#endif // TRANSFORMATION_H
