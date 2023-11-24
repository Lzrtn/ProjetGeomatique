#include <iostream>
#include <string>

#include <gdal/ogrsf_frmts.h>
#include <postgresql/libpq-fe.h>

// #include <ogr_api.h>
// #include <ogrsf_frmts.h>

#include <gdal/gdal.h>
#include <gdal/ogr_api.h>


#include "shapefile.h"
#include "transformation.h"

Shapefile::Shapefile(const std::string path):path(path)
{}

std::string Shapefile::getPath(){
    return (path);
}

std::string Shapefile::getTableName(){
    return (table_name);
}

int Shapefile::import_to_db(DbManager db_manager, const int epsg)
    {

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


    // Create the table in the database
        std::string request = "DROP TABLE IF EXISTS "+table_name+";";
        db_manager.CreateTable(request);
        std::string request_create = "CREATE TABLE "+table_name+" ();";
        db_manager.CreateTable(request_create);
        std::cout << "Table created\n";


    // Get the layer from the shapefile
    OGRLayer *poLayer = poDS->GetLayerByName(table_name.c_str());
    if (poLayer == nullptr) {
        std::cerr << "Failed to get layer from shapefile." << std::endl;
        //PQfinish(conn);
        GDALClose(poDS);
        return 1;
    }

    // Add features columns
    std::string instruction_add = "ALTER TABLE public."+table_name;
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
    db_manager.CreateTable(instruction_add);

    std::cout << "Columns added\n";

    // Loop through features and insert them into the database
    OGRFeature *poFeature;
    std::string instruction_fill = "";
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
                if (value.find("'") != std::string::npos){
                    std::replace(value.begin(), value.end(), '\'', ' ');
                }
                values += "'"+ value + "',";
            }
        }

        // Get the geometry
        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != nullptr) {
            // Convert the geometry to WKT (Well-Known Text)
            char *wkt;
            poGeometry->exportToWkt(&wkt);

            // Build the SQL INSERT statement
            instruction_fill += instruction + fields+"geom) VALUES("+values+"ST_GeomFromText('" + std::string(wkt) + "', "+std::to_string(epsg)+"));";

        }

        // Cleanup
        OGRFeature::DestroyFeature(poFeature);
    }
    db_manager.Request(instruction_fill);

    // Close the shapefile
    GDALClose(poDS);

    std::cout<<"IT WORKS!"<<std::endl;
    return 0;
}

std::vector<float> Shapefile::getBoundingBox(DbManager db_manager)
{
    std::string requete_SQL= "SELECT ST_Extent(geom) FROM "+table_name+";";
    db_manager.Request(requete_SQL);
    std::string resultat_SQL =db_manager.ParseResult();

    std::string bbx = resultat_SQL.substr(16, resultat_SQL.length() - 20);
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



QGraphicsItemGroup * Shapefile::plotShapefile(pqxx::result rowbis,QGraphicsScene *scene)
{


    Transformation t;

    QGraphicsItemGroup *layerGroup = new QGraphicsItemGroup();
    scene->addItem(layerGroup);
    for (const auto& rowbi : rowbis)
    {

        auto geojsongeom = rowbi[0].as<std::string>();
        std::string dataType = t.whatType(geojsongeom);

        if (dataType == "LineString" || dataType == "MultiLineString")
        {
            std::vector<QVector <QLineF>> segmentsToPlot = t.JSONtoCoordsLIN(geojsongeom);
            for(int i = 0; i< segmentsToPlot.size(); i++)
            {
                for (int j = 0; j< segmentsToPlot[i].size(); j++)
                {
                    QGraphicsLineItem *lineToPlotItem = new QGraphicsLineItem(segmentsToPlot[i][j]);
                    layerGroup->addToGroup(lineToPlotItem);
                }
            }

        }
        else if(dataType == "Polygon")
        {
            QPolygonF polygoneToPlot = t.JSONtoCoordsPOL(geojsongeom);
            QGraphicsPolygonItem *polygoneToPlotItem = new QGraphicsPolygonItem(polygoneToPlot);
            layerGroup->addToGroup(polygoneToPlotItem);
            QColor myColor = QColor("darkGreen");
            polygoneToPlotItem->setBrush(myColor);
        }
        else if(dataType == "Point" || dataType == "MultiPoint")
        {
            std::vector<QPointF> pointsToPlot = t.JSONtoCoordsPTS(geojsongeom);
            for(int i = 0; i< pointsToPlot.size(); i++)
            {
                QGraphicsEllipseItem *pointToPlotItem = new QGraphicsEllipseItem(pointsToPlot[i].x(),pointsToPlot[i].y(),10,10);
                layerGroup->addToGroup(pointToPlotItem);

            }
        }
    }
    return(layerGroup);
}
