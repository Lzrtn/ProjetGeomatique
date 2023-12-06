#ifndef SHAPEFILE_H
#define SHAPEFILE_H
#include <iostream>
#include <vector>
#include <pqxx/pqxx>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <QWidget>
#include <QPointF>
#include <QCheckBox>
#include <QColor>

#include "../outils/dbmanager.h"

/**
 * @brief Class representing a shapefile
 *
 * This class represents a shapefile, with its path, name, EPSG code, database manager, id and data type.
 **/
class Shapefile
{
public:
     /**
     * @brief Construct a new Shapefile object
     *
     * @param path Absolute path of your shapefile
     * @param db_manager DbManager of the database in which the shapefile wil be copied
     */
    Shapefile(std::string path, DbManager db_manager);

    /**
    * @brief Destruct a new Shapefile object
    */
   ~Shapefile();

    /**
     * @brief Get the path of the shapefile
     *
     * @returns std::string
     */
    std::string getPath();

    /**
     * @brief Get the name of the shapefile
     *
     * @returns std::string
     */
    std::string getTableName();

    /**
     * @brief Get the db manager of the shapefile
     *
     * @returns DbManager
     */
    DbManager getDbManager();

    /**
     * @brief Get the id of the shapefile
     *
     * @returns int
     */
    int getId();

    /**
    * @brief Copies the shapefile to a database
    *
    * @param epsg epsg code of your shapefile
    *
    * @returns integer : 0 if no error 1 if thereis an error
    */
    int import_to_db(const int epsg);

   /**
   * @brief Get the bounding box of the Shapefile
   *
   * @returns std::vector<float> [Xmin,Ymin,Xmax,Ymax]
   */
   std::vector<float> getBoundingBox();

   /**
   * @brief Plots the shapefile in the scene
   *
   * @param rowbis result of the postgresql request
   * @param rowbisType result of postgresql request giving different natures of features
   * @param rowTer giving natures
   * @param scene scene in which the shapefile is plot
   * @param myColor color of the layer
   *
   * @returns QgraphicsItemGroup
   */
   QGraphicsItemGroup * plotShapefile(pqxx::result rowbis, pqxx::result rowbistype,pqxx::result rowTer, QGraphicsScene *scene, QColor myColor);

   /**
   * @brief Gets the color stocked in the database
   *
   * @returns Qcolor
   */
   QColor showColor();

   /**
   * @brief Get the EPSG  code in a QString fomat
   *
   * @returns QString
   */
   QString getEPSGtoSet(){return EPSGtoSet;}

   /**
   * @brief Get the type of geometry in the shapefile
   *
   * @returns std::string data type written like in a geojson
   */
   std::string getDataType(){return data_type;}

private:

    std::string path; ///< Path of your shapefile

    std::string table_name; ///< Name of the table in which the shapefile is copied

    DbManager db_manager;  ///< DB manager in which the shapefile is stocked

    QString EPSGtoSet; ///< DB manager in which the shapefile is stocked

    int id; ///< Id of the shapefile in the table symbologie

    std::string data_type; ///< Type of the geometry

};

#endif // SHAPEFILE_H
