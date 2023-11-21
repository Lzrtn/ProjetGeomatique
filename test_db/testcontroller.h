#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H
#include <iostream>
#include "pqxx/pqxx"

class TestController
{
public:
    TestController(std::string db);
    TestController(std::string db, std::string ip);
    void CreateDb(std::string dbName);
    void CreateTable(std::string tableName);
    void Request(std::string request);
    std::string getString();
    std::string ParseResult();
    std::vector<std::vector<std::string>> ArrayParseResult();
private:
    std::string connString;
    pqxx::result result;
};

#endif // TESTCONTROLLER_H
