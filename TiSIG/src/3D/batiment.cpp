#include "batiment.h"
#include "outils/dbmanager.h"
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

    QVector3D base = QVector3D(1839252.78, 5174127.85, 284.483);

    std::vector<std::vector<std::vector<double>>> listPolygons;

    std::vector<QVector3D> coordinates;

    std::vector<std::vector<QVector3D>> listBatiments;

    DbManager manager("CityGML",ipAddress);


    try {


        pqxx::connection conn(manager.getString());

        if (conn.is_open()) {

            std::cout << "Requête un bâtiment" << std::endl;
            manager.Request("SELECT id, ST_ASGEOJSON(geometry) FROM surface_geometry WHERE parent_id = '" + id + "';");

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


    for(auto& polygon: listPolygons)

    {

        std::vector<QVector3D> listCoords = {};



        //REMPLIR LISTE COORDS

        for(auto& point: polygon){listCoords.push_back(QVector3D(point[0], point[1], point[2]));}

        translateToBasePoint(listCoords, base);

        while(listCoords.size() > 2 )

        {

            coordinates.push_back(QVector3D(listCoords[0][0], -listCoords[0][2], listCoords[0][1]));

            coordinates.push_back(QVector3D(listCoords[0+1][0], -listCoords[0+1][2], listCoords[0+1][1]));

            coordinates.push_back(QVector3D(listCoords[listCoords.size()-1][0], -listCoords[listCoords.size()-1][2], listCoords[listCoords.size()-1][1]));


            listCoords.erase(listCoords.begin() + 0);

        }


    }

    if (!coordinates.empty()) {
        QVector3D firstCoordinate = coordinates[0];
        std::cout << "Premier élément de coordinates : " << firstCoordinate.x() << ", " << firstCoordinate.y() << ", " << firstCoordinate.z() << std::endl;
    } else {
        std::cout << "Le vecteur coordinates est vide." << std::endl;
    }

    return coordinates;

}
