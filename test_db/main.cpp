#include <iostream>
#include <string>
#include <algorithm>
#include <pqxx/pqxx>
#include <limits.h>
#include <unistd.h>

// #include "dbcontroller.h"
#include "dbmanager.h"
#include "executor.h"
#include "docker.h"



int main() {
    try {
        // Creating container
        std::string pathDockerFile = "../docker-compose.yml";
        Docker docker(pathDockerFile);
        std::string ipAdress = docker.getIpAdress();
        // PostGreSQL Connection to the first database
        DbManager test("database2D", ipAdress);
        std::string connString = test.getString();
        std::cout << connString << std::endl;
        pqxx::connection conn(connString);

        if (conn.is_open()) {
            std::cout << "Connexion réussie à PostgreSQL" << std::endl;

            // Creation of a second database
            test.CreateDb("dbcreate_test");

            // Connection to the new database
            DbManager newTest("dbcreate_test", ipAdress);

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
/*
int main() {

    try {
        std::string pathDockerFile = "../docker-compose.yml";
        Docker docker(pathDockerFile);
        std::string ipAdress = docker.getIpAdress();
        std::string database = "database2D";

        std::cout << "connectionString" << std::endl;

        DbManager db_contr(database, ipAdress);
        std::cout << "après" << std::endl;
        std::string connectionString = db_contr.getString();

        std::cout << connectionString << std::endl;

        pqxx::connection conn(connectionString);
        std::cout << "connexion" << std::endl;

        db_contr.create("DROP DATABASE IF EXISTS testdb;", conn);
        db_contr.create("CREATE DATABASE testdb;", conn);
        db_contr.create("DROP TABLE IF EXISTS test_table; CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT);", conn);


        db_contr.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James_Bond','37');", conn);
        db_contr.Request("SELECT * FROM test_table;", conn);

        std::string result = db_contr.getResult();
        std::cout << result << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "error "<< std::endl;
        std::cerr << e.what() << std::endl;

        return 1;
    }


    return 0;
}
*/
