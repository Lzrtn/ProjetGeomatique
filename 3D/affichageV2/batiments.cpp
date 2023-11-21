#include "batiments.h"

Batiments::Batiments()
{

}

using json = nlohmann::json;

void Batiments::setListParentID()
{

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
            std::cout << "Requête liste parent_id" << std::endl;
            pqxx::work txn(conn);
            pqxx::result parentIDquery = txn.exec("SELECT DISTINCT parent_id FROM surface_geometry WHERE parent_id NOTNULL AND geometry NOTNULL LIMIT 2");

            for (const auto& row : parentIDquery) {
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

std::vector<std::string> Batiments::getListParentID()
{
    setListParentID();
    return listParentID;
}
