#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <string>
#include "../data/json.hpp"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>


/**
 * @brief Class representing a Transformation
 *
 * This class represents a transformation.
 **/
class Transformation
{
public:
    /**
    * @brief Construct a new Transformation object
    */
    Transformation();

    /**
    * @brief Destruct a new Transformation object
    */
    ~Transformation();

    /**
    * @brief Construct a QPolygonF that can be plotted
    *
    * @param polygon : a std::string that contains the polygon data
    *
    * @returns QPolygonF
    *
    */
    QPolygonF JSONtoCoordsPOL(std::string polygone);

    /**
    * @brief Construct a vector containing QLineF that can be plotted
    *
    * @param line : a std::string that contains the line data
    *
    * @returns std::vector
    *
    */
    std::vector<QVector <QLineF>> JSONtoCoordsLIN(std::string line);

    /**
    * @brief Construct a vector containing QPointF that can be plotted
    *
    * @param point : a std::string that contains the points data
    *
    * @returns std::vector
    *
    */
    std::vector<QPointF> JSONtoCoordsPTS(std::string point);

    /**
    * @brief Search the EPSG in the shapefile
    *
    * @param data : global data of a shapefile
    *
    * @returns std::string
    *
    */

    std::string whichCRS(std::string data);

    /**
    * @brief Search the type of features in the shapefile
    *
    * @param data : global data of a shapefile
    *
    * @returns std::string
    *
    */
    std::string whatType(std::string data);
};

#endif // TRANSFORMATION_H
