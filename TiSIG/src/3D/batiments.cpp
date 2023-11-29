#include "batiments.h"
#include "outils/dbmanager.h"

Batiments::Batiments()
{

}

using json = nlohmann::json;

void Batiments::setListParentID(std::string ipAddress)
{

    try {        
        DbManager manager("CityGML",ipAddress);
        pqxx::connection conn(manager.getString());

        if (conn.is_open()) {
            std::cout << "Requête liste parent_id" << std::endl;
            manager.Request("SELECT parent_id FROM surface_geometry JOIN cityobject ON cityobject.id = surface_geometry.cityobject_id WHERE ST_Intersects(geometry, ST_MakeEnvelope(1839181.39, 5173762.71, 1839482.04, 5174160.04, 4171)) AND cityobject.objectclass_id = '25' GROUP BY parent_id");
            for (const auto& row : manager.getResult()) {
                listParentID.push_back(row[0].as<std::string>());
           }

            conn.disconnect();
        } else {
            std::cerr << "Impossible de se connecter à la base de données." << std::endl;

        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;

    }
}

void Batiments::setListParentID(std::string ipAddress, std::string min_lon, std::string min_lat, std::string max_lon, std::string max_lat)
{

    try {
        DbManager manager("CityGML",ipAddress);
        pqxx::connection conn(manager.getString());

        if (conn.is_open()) {
            std::cout << "Requête liste parent_id" << std::endl;
            manager.Request("SELECT parent_id FROM surface_geometry JOIN cityobject ON cityobject.id = surface_geometry.cityobject_id WHERE ST_Intersects(geometry, ST_MakeEnvelope(" + min_lon + ", " + min_lat + ", " + max_lon + ", " + max_lat + ", " + "4171)) AND cityobject.objectclass_id = '25' GROUP BY parent_id");
            for (const auto& row : manager.getResult()) {
                listParentID.push_back(row[0].as<std::string>());
           }

            conn.disconnect();
        } else {
            std::cerr << "Impossible de se connecter à la base de données." << std::endl;

        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;

    }
}

std::vector<std::string> Batiments::getListParentID(std::string ipAddress)
{
    setListParentID(ipAddress);
    return listParentID;
}

std::vector<std::string> Batiments::getListParentID(std::string ipAddress, std::string min_lon, std::string min_lat, std::string max_lon, std::string max_lat){
    setListParentID(ipAddress, min_lon, min_lat, max_lon, max_lat);
    return listParentID;
}

std::map<int, Building3DFactory> Batiments::getBuildingMap(std::string ipAddress, std::string min_lon, std::string min_lat, std::string max_lon, std::string max_lat){
    std::map<int, Building3DFactory> mp;

    std::vector<std::string> listIndex = this->getListParentID( ipAddress, min_lon, min_lat, max_lon, max_lat);

    Batiments bats;

    std::vector<std::string> listParentID = bats.getListParentID(ipAddress);

    std::vector<std::vector<glm::vec3>> listBats;

    std::vector<QVector2D> textCoord =  {
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1, 1},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1, 1}
    };

    std::string textPath = ":/rose des vents.png";

    for (std::string idx: listParentID) {
        Batiment bat;
        std::vector<QVector3D> position = bat.createBuilding(idx,ipAddress);
        Building3DFactory building(position, position, textCoord, textPath);

        mp.emplace(std::stoi(idx), building);
    }

    return mp;

}
