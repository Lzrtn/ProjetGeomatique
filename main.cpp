#include <iostream>
#include <pqxx/pqxx>
#include "dbcontroller.h"

int main() {
    try {
        // PostGreSQL Connection to the first database
        DbController database_controller("database");

        if (database_controller.IsOpen()) {
            std::cout << "Connexion réussie à PostgreSQL" << std::endl;

            // Creation of a second database
            database_controller.Request("CREATE DATABASE testdb");

            // Connection to the new database
            DbController testdb_controller("testdb");

            // Creation of a table in the new database
            testdb_controller.Request("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");

            // Testing INSERT
            testdb_controller.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James_Bond','37')");

            // Testing SELECT
            testdb_controller.Request("SELECT * FROM test_table");
            std::string result = testdb_controller.getResult();

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
