#ifndef BATIMENTS_H
#define BATIMENTS_H
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "json.hpp"

class Batiments
{
public:
    Batiments();
    std::vector<std::string> getListParentID();
    void setListParentID();

private:
    std::vector<std::string> listParentID;
};

#endif // BATIMENTS_H
