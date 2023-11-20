#ifndef SHAPEFILE_H
#define SHAPEFILE_H
#include <iostream>


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
    int import_to_db(const std::string db_name,const std::string db_user,
            const std::string db_password,const std::string db_host,
            const std::string db_port, const int epsg);
private:
    /**
     * @brief Absolute or relative path of your shapefile
     */
    std::string path;
};

#endif // SHAPEFILE_H
