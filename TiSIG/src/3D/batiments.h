#ifndef BATIMENTS_H
#define BATIMENTS_H
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "json.hpp"
#include "batiment.h"

class Batiments
{
public:
    Batiments();
    std::vector<std::string> getListParentID(std::string ipAddress);
    std::vector<std::string> getListParentID(std::string ipAddress, std::string min_lon, std::string min_lat, std::string max_lon, std::string max_lat);
    void setListParentID(std::string ipAddress);
    void setListParentID(std::string ipAddress, std::string min_lon, std::string min_lat, std::string max_lon, std::string max_lat);
    std::map<int, Building3DFactory> getBuildingMap(std::string ipAddress, std::string min_lon, std::string min_lat, std::string max_lon, std::string max_lat);
private:
    std::vector<std::string> listParentID;
};

#endif // BATIMENTS_H
