#include "dbcontroller.h"
/**
 * @brief Constructor of DbController class
 * @param database The database to which the controller has to connect to
 */
DbController::DbController(std::string database){
    std::string conn_txt = "dbname="+database+" user=postgres password=postgres host=172.17.0.2 port=5432";
    pqxx::connection connection(conn_txt);
}
DbController::DbController(std::string database, std::string ip){
    std::string conn_txt = "dbname"+database+" user=postgres password=postgres host"+ip+" port=5432";
    pqxx::connection connection(conn_txt);
}
/**
 * @brief Function to execute a request
 *
 * Takes an SQL request in string format and executes it, then stores the result in the result attribute
 * @param request The request to be executed, in string format
 */
void DbController::Request(std::string request){
    pqxx::work txn(connection);
    result = txn.exec(request);
    txn.commit();
}
/**
 * @brief Retrieves the result of the last executed query, parses it and convert it to string.
 * @return data The rsult of the last query executed, parsed and converted to string.
 */
std::string DbController::getResult(){
    std::string data;
    for (pqxx::row row : result){
        for (int i = 0;i < row.size();i++){
            data.append(row[i].c_str());
        }
    }
    return data;
}
