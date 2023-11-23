#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <iostream>
#include "pqxx/pqxx"

class DbManager
{
public:
    /**
     * @brief Default constructor
     */
    DbManager();
    /**
     * @brief DbManager constructor
     * @param db Name of the database to connect to
     */
    DbManager(std::string db);
    /**
     * @brief DbManager constructor
     * @param db Name of the database to connect to
     * @param ip IP address where to find the database
     */
    DbManager(std::string db, std::string ip);
    /**
     * @brief Creates a new database on the Postgres server
     * @param dbName Name of the database to create
     */
    void CreateDb(std::string dbName);
    /**
     * @brief Creates a new table in the current database
     * @param tableName Name of the table to create
     */
    void CreateTable(std::string tableName);
    /**
     * @brief Send a SQL request to the current database and stores the result
     * @param request String of the request to execute
     */
    void Request(std::string request);
    /**
     * @brief Return connexion string to connect the database
     * @return connection string to connect
     */
    std::string getString();
    /**
     * @brief Parses the result of the last query and returns it in std::string format
     * @return results in a single string
     */
    std::string ParseResult();
    /**
     * @brief Parses the result ofthe last query and returns it in an array format.
     * @return vector of vector of string containing the result
     */
    std::vector<std::vector<std::string>> ArrayParseResult();
    /**
     * @brief Get the result of the request
     * @return Result of the request
     */
    pqxx::result getResult();

    /**
     * @brief connString std::string object containing the data to connect to a database
     */
    std::string connString;
    /**
     * @brief result pqxx::object containing the result of tha last query executed
     */
    pqxx::result result;
};

#endif // TESTCONTROLLER_H
