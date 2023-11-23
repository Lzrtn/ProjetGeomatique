#include "batiment.h"

void translateToBasePoint(std::vector<glm::vec3>& vectorToTranslate, const glm::vec3& basePoint) {
    for (auto& element : vectorToTranslate) {
        element -= basePoint;
    }
}


Batiment::Batiment()
{

}

std::vector<glm::vec3> Batiment::createBuilding(std::string id)
{

    using json = nlohmann::json;
    glm::vec3 base = glm::vec3(1838298.56, 5173585.46, 264.483);

    std::vector<std::vector<std::vector<double>>> listPolygons;
    std::vector<glm::vec3> coordinates;
    std::vector<std::vector<glm::vec3>> listBatiments;

    try {
        // Définir les paramètres de connexion
        const std::string dbname = "CityGML";
        const std::string user = "postgres";
        const std::string password = "postgres";
        const std::string hostaddr = "127.0.0.1";  // Utilisez "localhost" pour une connexion locale
        const std::string port = "5432";  // Utilisez "5432" pour le port par défaut

        // Construire la chaîne de connexion
        std::string conn_str = "dbname=" + dbname +
                               " user=" + user +
                               " password=" + password +
                               " hostaddr=" + hostaddr +
                               " port=" + port;

        // Se connecter à la base de données
        pqxx::connection conn(conn_str);

        if (conn.is_open()) {
            std::cout << "Requête un bâtiment" << std::endl;
            pqxx::work txn(conn);
            pqxx::result result = txn.exec("SELECT id, ST_ASGEOJSON(geometry) FROM surface_geometry WHERE parent_id = '" + id + "';");

            for (const auto& row : result) {

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
        std::vector<glm::vec3> listCoords = {};

        //REMPLIR LISTE COORDS
        for(auto& point: polygon){listCoords.push_back(glm::vec3(point[0], point[1], point[2]));}
        translateToBasePoint(listCoords, base);

        while(listCoords.size() > 3 )
        {

            std::vector<double> listAngles = {};
            int idxAngleMin = 0;
            for(int sizePolygon = 0; sizePolygon < listCoords.size(); sizePolygon++)
            {

                if(sizePolygon == 0)
                {
                    double xAv = listCoords[listCoords.size()-1][0];
                    double yAv = listCoords[listCoords.size()-1][1];

                    double x = listCoords[sizePolygon][0];
                    double y = listCoords[sizePolygon][1];

                    double xAp = listCoords[sizePolygon+1][0];
                    double yAp = listCoords[sizePolygon+1][1];

                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
                    if(angle < M_PI && angle > 0)
                    {
                        listAngles.push_back(angle);
                    }

                }

                if(sizePolygon > 0 && sizePolygon < listCoords.size()-2)
                {
                    double xAv = listCoords[sizePolygon-1][0];
                    double yAv = listCoords[sizePolygon-1][1];

                    double x = listCoords[sizePolygon][0];
                    double y = listCoords[sizePolygon][1];

                    double xAp = listCoords[sizePolygon+1][0];
                    double yAp = listCoords[sizePolygon+1][1];

                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
                    if(angle < M_PI && angle > 0)
                    {
                        listAngles.push_back(angle);
                    }

                }


                if(sizePolygon == listCoords.size()-1)
                {
                    double xAv = listCoords[sizePolygon-1][0];
                    double yAv = listCoords[sizePolygon-1][1];

                    double x = listCoords[sizePolygon][0];
                    double y = listCoords[sizePolygon][1];

                    double xAp = listCoords[0][0];
                    double yAp = listCoords[0][1];

                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
                    if(angle < M_PI && angle > 0)
                    {
                        listAngles.push_back(angle);
                    }
                }

                    auto it = std::find(listAngles.begin(), listAngles.end(), *min_element(listAngles.begin(), listAngles.end()));
                    if (it != listAngles.end()) {
                        idxAngleMin = std::distance(listAngles.begin(), it);
                    }


                    if(idxAngleMin == 0)
                    {
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][2], listCoords[idxAngleMin][1]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin+1][0], listCoords[idxAngleMin+1][2], listCoords[idxAngleMin+1][1]));
                        coordinates.push_back(glm::vec3(listCoords[listCoords.size()-1][0], listCoords[listCoords.size()-1][2], listCoords[listCoords.size()-1][1]));

                    }

                    if(idxAngleMin > 0 && idxAngleMin < listCoords.size()-2)
                    {
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][2], listCoords[idxAngleMin][1]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin+1][0], listCoords[idxAngleMin+1][2], listCoords[idxAngleMin+1][1]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin-1][0], listCoords[idxAngleMin-1][2], listCoords[idxAngleMin-1][1]));
                    }

                    if(sizePolygon == listCoords.size()-1)
                    {
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][2], listCoords[idxAngleMin][1]));
                        coordinates.push_back(glm::vec3(listCoords[0][0], listCoords[0][2], listCoords[0][1]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin-1][0], listCoords[idxAngleMin-1][2], listCoords[idxAngleMin-1][1]));
                    }

                    listCoords.erase(listCoords.begin() + idxAngleMin);

            }
        }

    }

    return coordinates;
}


//std::vector<glm::vec3> Batiment::getBuildingFromEmprise(double min_lon, double min_lat, double max_lon, double max_lat)
//{

//    using json = nlohmann::json;
//    glm::vec3 base = glm::vec3(1838315.89820259, 5173636.68599105, 264.483);

//    std::vector<std::vector<std::vector<std::vector<double>>>> listBatiments;
//    std::vector<std::vector<std::vector<double>>> listPolygons;
//    std::vector<glm::vec3> coordinates;

//    try {
//        // Définir les paramètres de connexion
//        const std::string dbname = "CityGML";
//        const std::string user = "postgres";
//        const std::string password = "postgres";
//        const std::string hostaddr = "127.0.0.1";  // Utilisez "localhost" pour une connexion locale
//        const std::string port = "5432";  // Utilisez "5432" pour le port par défaut

//        // Construire la chaîne de connexion
//        std::string conn_str = "dbname=" + dbname +
//                               " user=" + user +
//                               " password=" + password +
//                               " hostaddr=" + hostaddr +
//                               " port=" + port;

//        // Se connecter à la base de données
//        pqxx::connection conn(conn_str);

//        if (conn.is_open()) {
//            std::cout << "Requête de tous les bâtiments" << std::endl;
//            pqxx::work txn(conn);
//            pqxx::result result = txn.exec("SELECT parent_id, STRING_AGG(ST_ASGEOJSON(geometry)::text, ', ') AS geometries FROM surface_geometry WHERE ST_Intersects(geometry, ST_MakeEnvelope(1839465.08, 5173984.03, 1839464.88, 5173874.04, 4171)) GROUP BY parent_id LIMIT 20;");

//            for (const auto &row : result) {

//                }

//            for (const auto& row : result) {

////                std::cout << row.as<std::string>().split(',');
//                json jsonListPolygons = json::parse(row[1].as<std::string>())["coordinates"];

//                //json polygonUVCoordinates = json::parse(row[2].as<std::string>())["coordinates"][0];

//                for(auto& element: jsonListPolygons)
//                {
//                    std::vector<std::vector<double>> el = element;
//                    listPolygons.push_back(el);
//                }
//           }

//            conn.disconnect();
//        } else {
//            std::cerr << "Impossible de se connecter à la base de données." << std::endl;

//        }
//    } catch (const std::exception &e) {
//        std::cerr << "Erreur : " << e.what() << std::endl;
//    }

//    for(auto& polygon: listPolygons)
//    {
//        std::vector<glm::vec3> listCoords = {};

//        //REMPLIR LISTE COORDS
//        for(auto& point: polygon){listCoords.push_back(glm::vec3(point[0], point[1], point[2]));}
//        translateToBasePoint(listCoords, base);

//        while(listCoords.size() > 3 )
//        {

//            std::vector<double> listAngles = {};
//            int idxAngleMin = 0;
//            for(int sizePolygon = 0; sizePolygon < listCoords.size(); sizePolygon++)
//            {

//                if(sizePolygon == 0)
//                {
//                    double xAv = listCoords[listCoords.size()-1][0];
//                    double yAv = listCoords[listCoords.size()-1][1];

//                    double x = listCoords[sizePolygon][0];
//                    double y = listCoords[sizePolygon][1];

//                    double xAp = listCoords[sizePolygon+1][0];
//                    double yAp = listCoords[sizePolygon+1][1];

//                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
//                    if(angle < M_PI && angle > 0)
//                    {
//                        listAngles.push_back(angle);
//                    }

//                }

//                if(sizePolygon > 0 && sizePolygon < listCoords.size()-2)
//                {
//                    double xAv = listCoords[sizePolygon-1][0];
//                    double yAv = listCoords[sizePolygon-1][1];

//                    double x = listCoords[sizePolygon][0];
//                    double y = listCoords[sizePolygon][1];

//                    double xAp = listCoords[sizePolygon+1][0];
//                    double yAp = listCoords[sizePolygon+1][1];

//                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
//                    if(angle < M_PI && angle > 0)
//                    {
//                        listAngles.push_back(angle);
//                    }

//                }


//                if(sizePolygon == listCoords.size()-1)
//                {
//                    double xAv = listCoords[sizePolygon-1][0];
//                    double yAv = listCoords[sizePolygon-1][1];

//                    double x = listCoords[sizePolygon][0];
//                    double y = listCoords[sizePolygon][1];

//                    double xAp = listCoords[0][0];
//                    double yAp = listCoords[0][1];

//                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
//                    if(angle < M_PI && angle > 0)
//                    {
//                        listAngles.push_back(angle);
//                    }
//                }

//                    auto it = std::find(listAngles.begin(), listAngles.end(), *min_element(listAngles.begin(), listAngles.end()));
//                    if (it != listAngles.end()) {
//                        idxAngleMin = std::distance(listAngles.begin(), it);
//                    }


//                    if(idxAngleMin == 0)
//                    {
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][2], listCoords[idxAngleMin][1]));
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin+1][0], listCoords[idxAngleMin+1][2], listCoords[idxAngleMin+1][1]));
//                        coordinates.push_back(glm::vec3(listCoords[listCoords.size()-1][0], listCoords[listCoords.size()-1][2], listCoords[listCoords.size()-1][1]));

//                    }

//                    if(idxAngleMin > 0 && idxAngleMin < listCoords.size()-2)
//                    {
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][2], listCoords[idxAngleMin][1]));
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin+1][0], listCoords[idxAngleMin+1][2], listCoords[idxAngleMin+1][1]));
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin-1][0], listCoords[idxAngleMin-1][2], listCoords[idxAngleMin-1][1]));
//                    }

//                    if(sizePolygon == listCoords.size()-1)
//                    {
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][2], listCoords[idxAngleMin][1]));
//                        coordinates.push_back(glm::vec3(listCoords[0][0], listCoords[0][2], listCoords[0][1]));
//                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin-1][0], listCoords[idxAngleMin-1][2], listCoords[idxAngleMin-1][1]));
//                    }

//                    listCoords.erase(listCoords.begin() + idxAngleMin);

//            }
//        }

//    }

//    return coordinates;
//}

