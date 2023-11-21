#include <iostream>
#include <pqxx/pqxx>
#include "dbmanager.h"

int main() {
    try {
        // PostGreSQL Connection to the first database
        DbManager test("database2D");
        pqxx::connection conn(test.getString());

        if (conn.is_open()) {
            std::cout << "Connexion réussie à PostgreSQL" << std::endl;

            // Creation of a second database
            test.CreateDb("dbcreate_test");

            // Connection to the new database
            DbManager newTest("dbcreate_test");

            // Creation of a table in the new database
            newTest.CreateTable("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");

            // Testing INSERT
            newTest.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James_Bond','37')");
            newTest.Request("INSERT INTO test_table (id, nom, age) VALUES ('1654','Serge Botton','99')");

            // Testing SELECT
            newTest.Request("SELECT * FROM test_table");
            std::string result = newTest.ParseResult();

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
