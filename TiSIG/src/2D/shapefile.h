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

   QGraphicsItemGroup * plotShapefile(pqxx::result rowbis, QGraphicsScene *scene, QColor myColor);

   QColor showColor();

   QString getEPSGtoSet(){return EPSGtoSet;}

   std::string getDataType(){return data_type;}

private:
    /**
     * @brief Absolute or relative path of your shapefile
     */
    std::string path;

    /**
     * @brief Name of the table in which the shapefile is copied
     */
    std::string table_name;

    /**
    * @brief DB manager in which the shapefile is stocked
    */
   DbManager db_manager;

   /**
   * @brief DB manager in which the shapefile is stocked
   */
   QString EPSGtoSet;

   /**
   * @brief id of the shapefile in the table sympologie
   */
   int id;

   /**
   * @brief itype of the geometry
   */
   std::string data_type;

};

#endif // SHAPEFILE_H
