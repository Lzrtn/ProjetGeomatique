#include <iostream>
#include <string>
#include <gdal/ogrsf_frmts.h>
#include <postgresql/libpq-fe.h>
#include <string>
#include <ogr_api.h>
#include <ogrsf_frmts.h>

#include "shapefile.h"

int main() {
    std::string path1 = "ITINERAIRE_AUTRE.shp"; //Modify this line with the path of your shapefile
    Shapefile essai1 = Shapefile(path1);
    std::string path2 = "ARRONDISSEMENT.shp"; //Modify this line with the path of your shapefile
    Shapefile essai2 = Shapefile(path2);
    std::string db_name = "essai_dbf";
    std::string db_user = "postgres";
    std::string db_password = "postgres";
    std::string db_host = "localhost";
    std::string db_port = "5432";
    essai1.import_to_db(db_name,db_user,db_password,db_host,db_port, 4326);
    essai2.import_to_db(db_name,db_user,db_password,db_host,db_port, 4326);
    return 0;
}
