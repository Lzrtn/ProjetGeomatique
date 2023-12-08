#include "batiment.h"
#include "../outils/dbmanager.h"
#include "map"

void translateToBasePoint(std::vector<QVector3D>& vectorToTranslate, const QVector3D& basePoint) {

    for (auto& element : vectorToTranslate) {

        element -= basePoint;

    }

}


Batiment::Batiment()

{


}


std::vector<QVector3D> Batiment::createBuilding(std::string id, std::string ipAddress)

{


    using json = nlohmann::json;

    //QVector3D base = QVector3D(1839252.78, 5174127.85, 284.483);

    std::vector<std::vector<std::vector<double>>> listPolygons;

    std::vector<QVector3D> coordinates;

    std::vector<std::vector<QVector3D>> listBatiments;

    DbManager manager("CityGML",ipAddress);


    try {


        pqxx::connection conn(manager.getString());

        if (conn.is_open()) {

            std::cout << "Requête un bâtiment" << std::endl;
            manager.Request("SELECT id, ST_ASGEOJSON(geometry) FROM surface_geometry WHERE cityobject_id = '" + id + "' AND geometry NOTNULL;");

            for (const auto& row : manager.getResult()) {

                json jsonListPolygons = json::parse(row[1].as<std::string>())["coordinates"];

                //json polygonUVCoordinates = json::parse(row[2].as<std::string>())["coordinates"][0];

                for(auto& element: jsonListPolygons)

                {
                    std::vector<std::vector<double>> el = element;

                    listPolygons.push_back(el);

                }
           }

            conn.disconnect();

        } else {

            std::cerr << "Impossible de se connecter à la base de données." << std::endl;


        }

    } catch (const std::exception &e) {

        std::cerr << "Erreur : " << e.what() << std::endl;

    }

    double hauteurSol = 1000;


        for(auto& polygon: listPolygons)

        {

            for(auto& point: polygon)

            {

                if(point[2] < hauteurSol)

                {

                    hauteurSol = point[2];

                }


            }


        }




    for(auto& polygon: listPolygons)

    {

        std::vector<QVector3D> listCoords = {};

        //REMPLIR LISTE COORDS
        for(int i = 0; i<polygon.size(); i++)
        {
//            std::cout << polygon[i][2] << "\n";
            listCoords.push_back(QVector3D(polygon[i][0], polygon[i][1], polygon[i][2]));
        }

        //translateToBasePoint(listCoords, base);

        while(listCoords.size() > 2 )

        {


            if(listCoords[0][2] == listCoords[1][2] && listCoords[0][2] == listCoords[listCoords.size() - 1][2])
            {

                if(listCoords[0][2] > hauteurSol+1){

                    coordinates.push_back(QVector3D(listCoords[0][0],  listCoords[0][1], -listCoords[0][2]));
                    coordinates.push_back(QVector3D(listCoords[1][0], listCoords[1][1], -listCoords[1][2]));
                    coordinates.push_back(QVector3D(listCoords[listCoords.size()-1][0], listCoords[listCoords.size()-1][1], -listCoords[listCoords.size()-1][2]));
                }
            } else {
                coordinates.push_back(QVector3D(listCoords[0][0],  listCoords[0][1], -listCoords[0][2]));
                coordinates.push_back(QVector3D(listCoords[1][0], listCoords[1][1], -listCoords[1][2]));
                coordinates.push_back(QVector3D(listCoords[listCoords.size()-1][0], listCoords[listCoords.size()-1][1], -listCoords[listCoords.size()-1][2]));
            }
            listCoords.erase(listCoords.begin());
        }


    }
//    //Initializing the request
//    std::string request = "INSERT INTO triangle (geom, parent_id) VALUES ";
//    //Extracting the triplets of coordinates
//    for (int i =0;i < coordinates.size(); i+= 3){
//        const glm::vec3& v1 = coordinates[i];
//        const glm::vec3& v2 = coordinates[i + 1];
//        const glm::vec3& v3 = coordinates[i + 2];

//        std::string polygon = "POLYGON Z((";
//        polygon += std::to_string(v1.x) + " " + std::to_string(v1.y) + " " + std::to_string(v1.z) + "," +
//                " " + std::to_string(v2.x) + " " + std::to_string(v2.y) + " " + std::to_string(v2.z) + "," +
//                " " + std::to_string(v3.x) + " " + std::to_string(v3.y) + " " + std::to_string(v3.z) + "," +
//                " " + std::to_string(v1.x) + " " + std::to_string(v1.y) + " " + std::to_string(v1.z) + "))";
//        //std::cout<<polygon;
//        request += "(ST_GeomFromText('"+polygon+"', 4171),"+id+"),";
//        //std::string request = "INSERT INTO triangle (geom, parent_id) VALUES (ST_GeomFromText('"+polygon+"',4171),+"+id+");";
//    }
//    request.erase(request.size() - 1);
//    request += ";";
//    //std::cout<<request<<std::endl;
//    manager.Request(request);

//    if (!coordinates.empty()) {
//        QVector3D firstCoordinate = coordinates[0];
//        std::cout << "Premier élément de coordinates : " << firstCoordinate.x() << ", " << firstCoordinate.y() << ", " << firstCoordinate.z() << std::endl;
//    } else {
//        std::cout << "Le vecteur coordinates est vide." << std::endl;
//    }
// std::cout << coordinates.size() << std::endl;

    return coordinates;

}
