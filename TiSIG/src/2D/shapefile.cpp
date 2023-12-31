#include <iostream>
#include <string>
#include <gdal/ogrsf_frmts.h>
#include <postgresql/libpq-fe.h>
#include <gdal/gdal.h>
#include <gdal/ogr_api.h>

#include "shapefile.h"
#include "transformation.h"

// Constructor
Shapefile::Shapefile(const std::string path, DbManager db_manager):path(path), db_manager(db_manager)
{}

// Destructor
Shapefile::~Shapefile(){
    if (table_name!=""){
        std::string drop_table = "DROP TABLE IF EXISTS "+table_name;
        db_manager.Request(drop_table);
        std::cout<<"table dropped"<<std::endl;
    }
}

std::string Shapefile::getPath(){
    return (path);
}

std::string Shapefile::getTableName(){
    return (table_name);
}

DbManager Shapefile::getDbManager(){
    return (db_manager);
}

int Shapefile::getId(){
    return (id);
}

int Shapefile::import_to_db(const int epsg)
    {

    std::string layer_name = path.substr(path.find_last_of("/")+1, path.find_last_of(".shp")-path.find_last_of("/")-4);
    table_name = layer_name;

    //Pass the table name to lower case
    std::transform(table_name.begin(), table_name.end(), table_name.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // Remove spaces, points and parentheses in the table name
    if (table_name.find(".") != std::string::npos){
        std::replace(table_name.begin(), table_name.end(), '.', '_');
    }
    if (table_name.find(" ") != std::string::npos){
        std::replace(table_name.begin(), table_name.end(), ' ', '_');
    }
    if (table_name.find("(") != std::string::npos || table_name.find(")") != std::string::npos){
        std::replace(table_name.begin(), table_name.end(), '(', '_');
        std::remove(table_name.begin(), table_name.end(), ')');
    }

    // Check if there is a table with the same name in the database
    std::string tableExists = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = '"+table_name+"');";
    db_manager.Request(tableExists);
    pqxx::result res = db_manager.getResult();
    int n =0;
    if(res[0][0].as<std::string>()=="t"){
        n+=1;
        table_name = table_name+ "_" + std::to_string(n);
        std::string tableExists = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = '"+table_name+"');";
        db_manager.Request(tableExists);
        res = db_manager.getResult();
    }
    while(res[0][0].as<std::string>()=="t"){
        n+=1;
        table_name = table_name.substr(0,table_name.find_last_of("_"))+ "_" + std::to_string(n);
        std::string tableExists = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = '"+table_name+"');";
        db_manager.Request(tableExists);
        res = db_manager.getResult();
    }

    // Initialize GDAL
    GDALAllRegister();

    // Open the shapefile
    const char *shapefile_path = path.c_str();
    GDALDataset *poDS = static_cast<GDALDataset*>(GDALOpenEx(shapefile_path, GDAL_OF_VECTOR, nullptr, nullptr, nullptr));
    if (poDS == nullptr) {
        std::cerr << "Failed to open shapefile." << std::endl;
        return 1;
    }

    else{
        // Create the table in the database
            std::cout << table_name << std::endl;
            std::string request = "DROP TABLE IF EXISTS "+table_name+";";
            db_manager.CreateTable(request);
            std::string request_create = "CREATE TABLE "+table_name+" ();";
            db_manager.CreateTable(request_create);
            std::cout << "Table created" << std::endl;


        // Get the layer from the shapefile
        OGRLayer *poLayer = poDS->GetLayerByName(layer_name.c_str());
        if (poLayer == nullptr) {
            std::cerr << "Failed to get layer from shapefile." << std::endl;
            GDALClose(poDS);
            return 1;
        }
        else{
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

            std::cout << "Columns added" << std::endl;

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

                        // Replace special characters
                        std::unordered_map<char, std::string> replacements = {
                            {'\'', " "},     // Apostrophe
                            {'\xe9', "e"},   // e accent aigu (é)
                            {'\xe8', "e"},   // e accent grave (è)
                            {'\xea', "e"},   // e accent circonflexe (ê)
                            {'\xeb', "e"},   // e tréma (ë)
                            {'\xe7', "c"},   // c cédille (ç)
                            {'\xe0', "a"},   // a accent grave (à)
                            {'\xe2', "a"},   // a accent circonflexe (â)
                            {'\xe4', "a"},   // a tréma (ä)
                            {'\xfb', "u"},   // u accent circonflexe (û)
                            {'\xfc', "u"},   // u tréma (ü)
                            {'\xef', "i"},   // i tréma (ï)
                            {'\xee', "i"},   // i accent circonflexe (î)
                            {'\xf4', "o"},   // o accent circonflexe (ô)
                            {'\xf6', "o"},   // o tréma (ö)
                            {'\xe4', "a"},   // a tréma (ä)
                            {'\xfb', "u"},   // u accent circonflexe (û)
                            {'\xfc', "u"},   // u tréma (ü)
                            {'\xef', "i"},   // i tréma (ï)
                            {'\xee', "i"},   // i accent circonflexe (î)
                            {'\xe4', "a"},   // a tréma (ä)
                            {'\xfb', "u"},   // u accent circonflexe (û)
                            {'\xfc', "u"},   // u tréma (ü)
                            {'\xef', "i"},   // i tréma (ï)
                            {'\xee', "i"},   // i accent circonflexe (î)
                            {'\xc9', "E"},   // E accent aigu (É)
                            {'\xc8', "E"},   // E accent grave (È)
                            {'\xca', "E"},   // E accent circonflexe (Ê)
                            {'\xcb', "E"},   // E tréma (Ë)
                            {'\xc7', "C"},   // C cédille (Ç)
                            {'\xc0', "A"},   // A accent grave (À)
                            {'\xc2', "A"},   // A accent circonflexe (Â)
                            {'\xc4', "A"},   // A tréma (Ä)
                            {'\xdb', "U"},   // U accent circonflexe (Û)
                            {'\xdc', "U"},   // U tréma (Ü)
                            {'\xcf', "I"},   // I tréma (Ï)
                            {'\xce', "I"},   // I accent circonflexe (Î)
                            {'\xd4', "O"},   // O accent circonflexe (Ô)
                            {'\xd6', "O"},   // O tréma (Ö)
                            // Ajoutez d'autres caractères spéciaux et leurs remplacements ici
                        };
                        for (const auto& [searchChar, replaceStr] : replacements) {
                            size_t pos = 0;
                            while ((pos = value.find(searchChar, pos)) != std::string::npos) {
                                value.replace(pos, 1, replaceStr);
                                pos += replaceStr.length(); // Passer au caractère suivant après le remplacement
                            }
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

            // Add color
            std::string name_symbo;
            std::cout<<std::to_string(idType)<<std::endl;
            if (idType==1000){name_symbo="symbologie_shp";}
            else if (idType==3000){name_symbo="symbologie_flow";}
            std::string tableSymbo = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = '"+name_symbo+"');";
            db_manager.Request(tableSymbo);
            pqxx::result r = db_manager.getResult();
            if(r[0][0].as<std::string>()=="f"){
                std::string request_create_symbo = "CREATE TABLE "+name_symbo+" ();";
                db_manager.CreateTable(request_create_symbo);
                std::string request_columns = "ALTER TABLE public."+name_symbo+" ADD COLUMN id int, ADD COLUMN name character varying, ADD COLUMN red int, ADD COLUMN green int, ADD COLUMN blue int, ADD COLUMN alpha int;";
                db_manager.CreateTable(request_columns);
            }
            std::string request_max_id = "SELECT MAX (id) FROM "+name_symbo+"";
            db_manager.Request(request_max_id);
            pqxx::result i = db_manager.getResult();
            int index;
            if (i[0][0].is_null()){
                index = idType;
            }
            else {
                index = i[0][0].as<int>() +1;
            }
            id=index;
            srand(time(NULL));
            int red_random = rand()%255;
            int green_random = rand()%255;
            int blue_random = rand()%255;
            std::string add_line_symbo = "INSERT INTO "+name_symbo+" (id, name, red, green, blue, alpha) VALUES ("+std::to_string(index)+",'"+table_name+"',"+std::to_string(red_random)+","+std::to_string(green_random)+","+std::to_string(blue_random)+",255);";
            db_manager.Request(add_line_symbo);
            std::cout<<table_name << " " << std::to_string(id)<<std::endl;
            std::cout<<"IT WORKS!"<<std::endl;
            return 0;
        }
    }
}


std::vector<float> Shapefile::getBoundingBox()
{
    // Get the bounding box using sql request
    std::string requete_SQL= "SELECT ST_Extent(geom) FROM "+table_name+";";
    db_manager.Request(requete_SQL);
    std::string resultat_SQL =db_manager.ParseResult();

    // Format the result to get X and Y values
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

    // Create a vector that contains the bounding box
    std::vector<float> res = {Xmin,Ymin,Xmax,Ymax};
    return res; // Return the vector
}



QGraphicsItemGroup * Shapefile::plotShapefile(pqxx::result rowbis,pqxx::result rowbisType, pqxx::result rowTer,QGraphicsScene *scene, QColor myColor)
{
    Transformation t; // Instanciation of transformation class
    QGraphicsItemGroup *layerGroup = new QGraphicsItemGroup();
    scene->addItem(layerGroup);

    // Getting the EPSG of the shapefile
    std::string EPSG;
    for (const auto& rowbi : rowbis)
    {
        auto geojsongeom = rowbi[0].as<std::string>();
        EPSG = t.whichCRS(geojsongeom).substr(5,7);
        break;
    }
    // Define the string to plot on the app
    EPSGtoSet = QString::fromStdString(EPSG);

    std::map<std::string, QColor> natureColorMap;
    if (rowbisType.empty()) {
        // rowbisType n'a pas été initiaisé ou est vide
        std::cout<<"On est dans le cas de l'emprise youhou"<<std::endl;
    } else {
        // Getting natures and the number of them
        int nbColor = 0; // Defining the number of color (= number of nature) that will be needed
        std::vector <std::string> natureList;
        for (const auto& rowte : rowTer)
        {
            std::string natureValue = rowte[0].as<std::string>();
            natureList.push_back(natureValue); // Create a list with every nature existing in the shapefile
            nbColor +=1;
        }

        // Generating colors, as many as there are different natures
        srand(time(NULL)); // Using classic random lib
        std::vector <QColor> colorList;
        for (int i = 0; i < nbColor; i++) {
            int red_random = rand() % 255;
            int green_random = rand() % 255;
            int blue_random = rand() % 255;
            QColor colorToAssign(red_random, green_random, blue_random);
            colorList.push_back(colorToAssign); // Create a list with as many color as there are natures
        }

        // Association of any nature to a specific color

        for (int i = 0; i < nbColor; ++i) {
            QColor color = colorList[i];
            natureColorMap[natureList[i]] = color;
        }

    }


    int count = 0;

    for (const auto& rowbi : rowbis)
    {
        std::string nature ;
        if (rowbisType.empty())
        {
            std::cout<<"C'est compliqué"<<std::endl;
        } else {
            nature = rowbisType[count][0].c_str(); // Get nature of the feature the iteration is about
        }

        count +=1;
        auto geojsongeom = rowbi[0].as<std::string>(); // Get the geometry of the feature
        std::string dataType = t.whatType(geojsongeom); // Get the type of geometry so the plot code is adapted
        data_type=dataType;
        if (dataType == "LineString" || dataType == "MultiLineString") // Case of the LineString or MultiLineString
        {
            std::vector<QVector <QLineF>> segmentsToPlot = t.JSONtoCoordsLIN(geojsongeom);
            for(int i = 0; i< (int)segmentsToPlot.size(); i++)
            {
                for (int j = 0; j< (int)segmentsToPlot[i].size(); j++)
                {
                    QGraphicsLineItem *lineToPlotItem = new QGraphicsLineItem(segmentsToPlot[i][j]);
                    layerGroup->addToGroup(lineToPlotItem);
                }
            }
        }
        else if(dataType == "Polygon") // Case of the Polygon
        {
            QPolygonF polygoneToPlot = t.JSONtoCoordsPOL(geojsongeom);
            QGraphicsPolygonItem *polygoneToPlotItem = new QGraphicsPolygonItem(polygoneToPlot);
            layerGroup->addToGroup(polygoneToPlotItem);
            if (natureColorMap.empty())
            {
                polygoneToPlotItem->setBrush(Qt::red);
            }else
            {
                QColor natureColor = natureColorMap[nature];

                polygoneToPlotItem->setBrush(natureColor);
            }

        }
        else if(dataType == "Point" || dataType == "MultiPoint") // Case of the Point or the MultiPoint
        {
            std::vector<QPointF> pointsToPlot = t.JSONtoCoordsPTS(geojsongeom);
            for(int i = 0; i< (int)pointsToPlot.size(); i++)
            {
                QGraphicsEllipseItem *pointToPlotItem = new QGraphicsEllipseItem(pointsToPlot[i].x(),pointsToPlot[i].y(),10,10);
                layerGroup->addToGroup(pointToPlotItem);

            }
        }
    }
    return(layerGroup);
}


QColor Shapefile::showColor(){
    std::string name_symbo;
    if (idType==1000){name_symbo="symbologie_shp";}
    else if (idType==3000){name_symbo="symbologie_flow";}
    std::string requete_couleur = "SELECT red, green, blue, alpha from "+name_symbo+" where name = '"+table_name+"';";
    db_manager.Request(requete_couleur);
    pqxx::result res = db_manager.getResult();
    int red = res[0][0].as<int>();
    int green = res[0][1].as<int>();
    int blue = res[0][2].as<int>();
    int alpha = res[0][3].as<int>();
    return (QColor(red,blue,green,alpha));

}


