#include "dbmanager.h"

DbManager::DbManager(std::string db){
    connString = "dbname="+db+" user=postgres password=postgres host=172.17.0.2 port=5432";
}
DbManager::DbManager(std::string db,std::string ip){
    connString = "dbname="+db+" user=postgres password=postgres host="+ip+" port=5432";
}
void DbManager::CreateDb(std::string dbName){
    pqxx::connection newConn(this->getString());
    pqxx::nontransaction txn(newConn);
    txn.exec("DROP DATABASE IF EXISTS "+dbName);
    txn.commit();
    pqxx::nontransaction txn2(newConn);
    txn2.exec("CREATE DATABASE "+dbName);
    txn2.commit();
}
void DbManager::CreateTable(std::string request){
    pqxx::connection newConn(this->getString());
    pqxx::nontransaction txn(newConn);
    txn.exec(request);
    txn.commit();
}
void DbManager::Request(std::string request){
    pqxx::connection newConn(this->getString());
    pqxx::work txn(newConn);
    pqxx::result r = txn.exec(request);
    txn.commit();
    result = r;
}
std::string DbManager::getString(){
    return connString;
}
std::string DbManager::ParseResult(){
    std::string parsed;
    for (const auto &row : result){
        for (const auto &field : row){
            std::cout << field.name() << " = " << field.c_str() << ", ";
            parsed.append(field.name());
            parsed.append(" = ");
            parsed.append(field.c_str());
            parsed.append(", ");
        }
        std::cout << std::endl;
        parsed.append("\n");
    }
    return parsed;
}
std::vector<std::vector<std::string>> DbManager::ArrayParseResult(){
    std::vector<std::vector<std::string>> parsedData;

    for (const auto &row : result) {
        std::vector<std::string> rowData;

        for (const auto &field : row) {
            std::cout << field.name() << " = " << field.c_str() << ", ";
            rowData.push_back(std::string(field.c_str()));
            }

        std::cout << std::endl;
        parsedData.push_back(rowData);
        }

    return parsedData;
}
