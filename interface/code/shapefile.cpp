#include <iostream>
#include <string>
#include <gdal/ogrsf_frmts.h>
#include <postgresql/libpq-fe.h>
#include <string>
#include <ogr_api.h>
#include <ogrsf_frmts.h>
#include <gdal/gdal.h>
#include <gdal/ogr_api.h>

#include "shapefile.h"

Shapefile::Shapefile(const std::string path):path(path)
{}

std::string Shapefile::getPath(){
    return (path);
}

std::string Shapefile::getTableName(){
    return (table_name);
}

int Shapefile::import_to_db(const std::string db_name,const std::string db_user,
const std::string db_password,const std::string db_host,const std::string db_port, const int epsg)
{
    this->db_name = db_name;
    this->db_user = db_user;
    this->db_password = db_password;
    this->db_host = db_host;
    this->db_port = db_port;

    std::string table_name = path.substr(path.find_last_of("/")+1, path.find_last_of(".shp")-path.find_last_of("/")-4);

    this->table_name = table_name;
    // Initialize GDAL
    GDALAllRegister();

    // Open the shapefile
    const char *shapefile_path = path.c_str();
    GDALDataset *poDS = static_cast<GDALDataset*>(GDALOpenEx(shapefile_path, GDAL_OF_VECTOR, nullptr, nullptr, nullptr));
    if (poDS == nullptr) {
        std::cerr << "Failed to open shapefile." << std::endl;
        return 1;
    }

    // Connect to the PostgreSQL/PostGIS database
    std::string conn_infos ="dbname="+db_name+" user="+db_user+" password="+db_password+" host="+db_host +" port="+db_port;
    const char* conninfo = conn_infos.c_str();
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        GDALClose(poDS);
        return 1;
    }


    // Create the table in the database
        const char* tableName =table_name.c_str();
        const char* createTableSQL = "CREATE TABLE IF NOT EXISTS %s ();";
        char createTableStatement[200];
        snprintf(createTableStatement, sizeof(createTableStatement), createTableSQL, tableName);

        PGresult *createTableResult = PQexec(conn, createTableStatement);

        if (PQresultStatus(createTableResult) != PGRES_COMMAND_OK) {
            std::cerr << "Failed to create table: " << PQresultErrorMessage(createTableResult) << std::endl;
            PQclear(createTableResult);
            PQfinish(conn);
            GDALClose(poDS);
            return 1;
        }
        PQclear(createTableResult);


    // Get the layer from the shapefile
    OGRLayer *poLayer = poDS->GetLayerByName(table_name.c_str());
    if (poLayer == nullptr) {
        std::cerr << "Failed to get layer from shapefile." << std::endl;
        PQfinish(conn);
        GDALClose(poDS);
        return 1;
    }

    // Add features columns
    std::string instruction_add = "ALTER TABLE IF EXISTS public."+table_name;
    OGRFeatureDefn* featureDefn = poLayer->GetLayerDefn();
    for (int i=0; i<featureDefn->GetFieldCount(); i++){
        OGRFieldDefn* fieldDefn = featureDefn->GetFieldDefn(i);
        const char* fieldName = fieldDefn->GetNameRef();
        std::string field_name = fieldName;
        OGRFieldType fieldType = fieldDefn->GetType();

        // Access attribute values based on field type
        std::string data_type;
        switch (fieldType) {
            case OFTInteger:
                data_type = "integer";
                break;
            case OFTIntegerList:
                data_type = "integer[]";
            break;
            case OFTReal:
                data_type = "double precision";
                break;
            case OFTRealList:
                data_type = "double precision[]";
            break;
            case OFTString:
                data_type = "character varying";
                break;
            case OFTStringList:
                data_type = "character varying[]";
            break;
            case OFTBinary:
                data_type = "geom";
                break;
            case OFTWideString:
                data_type = "text";
            break;
            case OFTWideStringList:
                data_type = "text[]";
            break;
            case OFTDate:
                data_type = "timestamp";
            break;
            case OFTDateTime:
                data_type = "timestamp";
            break;
            case OFTTime:
                data_type = "timestamp";
            break;
            default:
                printf("%s: [Unhandled Field Type]\n", fieldName);
                data_type = "character varying";
        }
        instruction_add += " ADD COLUMN "+field_name+" "+data_type+",";

    }
    // Build the SQL ADD COLUMN statement
    instruction_add += " ADD COLUMN geom geometry;";
    const char* addTableSQL = instruction_add.c_str();
    PGresult *addTableResult = PQexec(conn, addTableSQL);
    if (PQresultStatus(addTableResult) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to create table: " << PQresultErrorMessage(addTableResult) << std::endl;
        PQclear(addTableResult);
        PQfinish(conn);
        GDALClose(poDS);
        return 1;

    PQclear(addTableResult);
    }

    // Loop through features and insert them into the database
    OGRFeature *poFeature;
    while ((poFeature = poLayer->GetNextFeature()) != nullptr) {
        std::string instruction = "INSERT INTO "+table_name +"(" ;
        std::string fields;
        std::string values;
        // Access attributes
        for (int i=0; i<featureDefn->GetFieldCount(); i++){
            OGRFieldDefn* fieldDefn = featureDefn->GetFieldDefn(i);
            const char* fieldName = fieldDefn->GetNameRef();
            std::string field_name = fieldName;
            const char *fieldValue = poFeature->GetFieldAsString(fieldName);
            std::string value = fieldValue;
            if (!value.empty() && i !=featureDefn->GetFieldCount()-1){
                fields += field_name + ",";
                values += "'"+value + "',";
            }
        }

        // Get the geometry
        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != nullptr) {
            // Convert the geometry to WKT (Well-Known Text)
            char *wkt;
            poGeometry->exportToWkt(&wkt);

            // Build the SQL INSERT statement
            std::string instruction_fill = instruction + fields+"geom) VALUES("+values+"ST_GeomFromText('" + std::string(wkt) + "', "+std::to_string(epsg)+"));";
            const char* fillTableSQL = instruction_fill.c_str();
            PGresult *fillTableResult = PQexec(conn, fillTableSQL);
            if (PQresultStatus(fillTableResult) != PGRES_COMMAND_OK) {
                std::cerr << "Failed to create table: " << PQresultErrorMessage(fillTableResult) << std::endl;
                PQclear(fillTableResult);
                PQfinish(conn);
                GDALClose(poDS);
                return 1;
            }
            PQclear(fillTableResult);


            CPLFree(wkt);
        }


        // Cleanup
        OGRFeature::DestroyFeature(poFeature);
    }


    // Close the database connection
    PQfinish(conn);

    // Close the shapefile
    GDALClose(poDS);

    std::cout<<"IT WORKS!"<<std::endl;
    return 0;
}

std::vector<float> Shapefile::getBoundingBox()
{
    // Connect to the PostgreSQL/PostGIS database
    std::string conn_infos ="dbname="+db_name+" user="+db_user+" password="+db_password+" host="+db_host +" port="+db_port;
    const char* conninfo = conn_infos.c_str();
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
    }
    //const char * tableName = table_name.c_str();
    std::string requete_SQL= "SELECT ST_Extent(geom) FROM "+table_name+";";
    const char* requeteSQL = requete_SQL.c_str();

    PGresult *bBoxResult = PQexec(conn, requeteSQL);

    if (PQresultStatus(bBoxResult) != PGRES_TUPLES_OK) {
        std::cerr << "Failed to get bounding box: " << PQresultErrorMessage(bBoxResult) << std::endl;
        PQclear(bBoxResult);
        PQfinish(conn);
    }

    const char *value = PQgetvalue(bBoxResult, 0, 0);
    PQclear(bBoxResult);

    std::string resultat_SQL = value;
    std::string bbx = resultat_SQL.substr(4, resultat_SQL.length() - 2);
    std::string f_pt = bbx.substr(0, bbx.find(","));
    std::string s_pt = bbx.substr(bbx.find(",")+1, bbx.length());
    std::string X_min = f_pt.substr(0,f_pt.find(" "));
    std::string Y_min = f_pt.substr(f_pt.find(" ")+1,f_pt.length());
    std::string X_max = s_pt.substr(0,s_pt.find(" "));
    std::string Y_max = s_pt.substr(s_pt.find(" ")+1,s_pt.length());

    float Xmin = std::stof(X_min);
    float Ymin = std::stof(Y_min);
    float Xmax = std::stof(X_max);
    float Ymax = std::stof(Y_max);

    std::cout<<Xmin<<","<<Ymin<<","<<Xmax<<","<<Ymax<<std::endl;

    std::vector<float> res = {Xmin,Ymin,Xmax,Ymax};
    return res;
}
