#include "batiments.h"
#include "../outils/dbmanager.h"
#include "string.h"
#include <iomanip>
#include <sstream>

Batiments::Batiments(std::string ip)
    :ipAddress(ip)
{
}

using json = nlohmann::json;

void Batiments::GetObjectsInEmprise(
        const Emprise &emprise,
        std::map<int, Object3DFactory*> &new_objects,
        std::vector<int> &show_objects,
        std::vector<int> &forget_objects)
{

    std::vector<std::string> listIndex = getListParentID(emprise);

    //show_objects.clear();
    show_objects = {};
    forget_objects.clear();
    new_objects.clear();

    std::cout << "Contenu de listIndex : ";
       for (const auto& index : listIndex) {
           std::cout << index << " ";
       }
       std::cout << std::endl;

    for (const auto& index : listIndex) {
        int buildingID = stoi(index);

        show_objects.push_back(buildingID);

        // Check if buildingID is already in indexStorage
        if (std::find(indexStorage.begin(), indexStorage.end(), buildingID) == indexStorage.end()) {
            indexStorage.push_back(buildingID);

            auto buildingIt = buildings.find(buildingID);
            if (buildingIt != buildings.end()) {
                new_objects.emplace(buildingID, buildingIt->second);
            } else {
                std::vector<QVector3D> position = bat.createBuilding(index, ipAddress);
                Building3DFactory *building = new Building3DFactory(position, position, textCoord, textPath);
                buildings.emplace(buildingID, building);
                new_objects.emplace(buildingID, building);
            }
        }
    }

    for (auto it = buildings.begin(); it != buildings.end();) {
        int buildingID = it->first;
        if (std::find(show_objects.begin(), show_objects.end(), buildingID) == show_objects.end()) {
            forget_objects.push_back(buildingID);

            if (it->second) {
                delete it->second;
            }

            it = buildings.erase(it);
        } else {
            ++it;
        }
    }

    sort( show_objects.begin(), show_objects.end() );
    show_objects.erase( unique( show_objects.begin(), show_objects.end() ), show_objects.end() );

    std::cout << "Taille index : " << indexStorage.size() << std::endl;
    std::cout << "Taille show  : " << show_objects.size() << std::endl;
    std::cout << "Taille forget  : " << forget_objects.size() << std::endl;
    std::cout << "Taille new  : " << new_objects.size() << std::endl;
}



Batiments::~Batiments()
{
    for (auto pair: this->buildings) {
        delete pair.second;
    }

}


std::vector<std::string> Batiments::getListParentID(const Emprise &emprise)
{
    std::vector<std::string> listIDtoDraw;

    try {
        DbManager manager("CityGML", ipAddress);
        pqxx::connection conn(manager.getString());

        if (conn.is_open()) {

            std::cout << "Requête liste parent_id" << std::endl;

            std::ostringstream ss;
            ss << "SELECT parent_id FROM surface_geometry "
               << "JOIN cityobject ON cityobject.id = surface_geometry.cityobject_id "
               << "WHERE ST_Intersects(geometry, ST_Polygon(ST_GeomFromText('LINESTRING(";

            for (const auto& point : {emprise.g_a, emprise.g_b, emprise.g_c, emprise.g_d}) {

                ss << std::fixed << std::setprecision(6) << point.x() << " " << point.y() << ", ";
            }

            ss << std::fixed << std::setprecision(6) << emprise.g_a.x() << " " << emprise.g_a.y();

            ss << ")'), 4171)) AND cityobject.objectclass_id = '25' GROUP BY parent_id;";

            std::string query = ss.str();

            std::cout << query;

            manager.Request(query);


            for (const auto& row : manager.getResult()) {
                std::string id = row[0].as<std::string>();
                listIDtoDraw.push_back(row[0].as<std::string>());
            }

            conn.disconnect();
        } else {
            std::cerr << "Impossible de se connecter à la base de données." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return listIDtoDraw;

}

