#include <iostream>
#include <pqxx/pqxx>

int main() {
    try {
        // PostGreSQL Connection to the first database
        pqxx::connection conn("dbname=abcabc user=postgres password=postgres host=172.17.0.2 port=5432");

        if (conn.is_open()) {
            std::cout << "Connexion réussie à PostgreSQL" << std::endl;

            // Creation of a second database
            pqxx::nontransaction txn(conn);
            pqxx::result r = txn.exec("CREATE DATABASE jppjpp");
            txn.commit();

            // New connection to the new database
            pqxx::connection connex("dbname=jppjpp user=postgres password=postgres host=172.17.0.2 port=5432");

            // New transaction for the new connection
            pqxx::work txn2(connex);
            r = txn2.exec("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");
            txn2.commit();

            // New transaction for a SELECT request
            pqxx::work txn3(connex);
            r = txn3.exec("SELECT * FROM test_table");
            txn3.commit();

            // Displaying the result
            for (auto row : r) {
                std::cout << "id: " << row["id"].c_str() << ", nom: " << row["nom"].c_str() << ", age: " << row["age"].c_str() << std::endl;
            }
        } else {
            std::cout << "Échec de la connexion à PostgreSQL" << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
