#ifndef SHAPEFILE_H
#define SHAPEFILE_H
#include <iostream>
#include <vector>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <QWidget>
#include <QPointF>
#include <QCheckBox>
#include <QColor>
#include <pqxx/pqxx>

#include "../../test_db/dbmanager.h"

class Shapefile
{
public:
     /**
     * @brief Construct a new Shapefile object
     *
     * @param path Absolute or relative path of your shapefile
     */
    Shapefile(std::string path);


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
     * @brief Copies the shapefile to a database
     *
     * @param db_name name of the database
     * @param db_user username of the database
     * @param db_password password of the database
     * @param db_host host of the database
     * @param db_port port of the database
     * @param epsg epsg code of your shapefile
     *
     * @returns integer : 0 if no error 1 if thereis an error
     */
    /*int import_to_db(const std::string db_name,const std::string db_user,
            const std::string db_password,const std::string db_host,
            const std::string db_port, const int epsg);*/
    int import_to_db(DbManager db_manager,  const int epsg);

    /**
    * @brief Get the bounding box of the Shapefile
    *
    * @returns std::vector<float> [Xmin,Ymin,Xmax,Ymax]
    */
   std::vector<float> getBoundingBox(DbManager db_manager);

   QGraphicsItemGroup * plotShapefile(pqxx::result rowbis, QGraphicsScene *scene);

private:
    /**
     * @brief Absolute or relative path of your shapefile
     */
    std::string path;

    /**
     * @brief Name of the database in which the shapefile is copied
     */
    std::string db_name;

    /**
     * @brief Username of the database in which the shapefile is copied
     */
    std::string db_user;

    /**
     * @brief Password of the database in which the shapefile is copied
     */
    std::string db_password;

    /**
     * @brief Host of the database in which the shapefile is copied
     */
    std::string db_host;

    /**
     * @brief Port of the database in which the shapefile is copied
     */
    std::string db_port;

    /**
     * @brief Name of the table in which the shapefile is copied
     */
    std::string table_name;

};

#endif // SHAPEFILE_H