#ifndef BATIMENT_H
#define BATIMENT_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include <pqxx/pqxx>
#include "json.hpp"
#include "QVector3D"
#include "building3d.h"

class Batiment
{
public:
    Batiment();
    std::vector<QVector3D> createBuilding(std::string id, std::string ipAddress);
    std::vector<glm::vec3> getBuildingFromEmprise(double min_lon, double min_lat, double max_lon, double max_lat);
    std::map<int, Building3D> getBuildingMap(double min_lon, double min_lat, double max_lon, double max_lat);
private:
    std::string id;
};

#endif // BATIMENT_H
