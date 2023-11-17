#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H
#include <iostream>
#include <pqxx/pqxx>
/**
 * @brief The DbController class
 *
 * This class connects to a database in it's constructor, and facilitate the usage of SQL requests
 */
class DbController
{
public:
    /**
     * @brief DbController class constructor
     * @param database Name of the database to connect to
     */
    DbController(std::string database);
    /**
     * @brief Send an SQL request to the database and puts the result in the result attribute
     * @param request SQL request to execute
     */
    void Request(std::string request);
    /**
     * @brief Parses and returns the result under a string format.
     * @return The result of the last request executed
     */
    std::string getResult();
    /**
     * @brief Checks if the connection to the database is correct
     * @return true if the connection is open, false if not.
     */
    bool IsOpen(){
        return connection.is_open();
    }
private:
    pqxx::connection connection; ///< Connection token to the given database
    pqxx::result result; ///< Result of the last request executed
};

#endif // DBCONTROLLER_H
