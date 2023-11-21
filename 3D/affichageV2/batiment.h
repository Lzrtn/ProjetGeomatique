#ifndef BATIMENT_H
#define BATIMENT_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include <pqxx/pqxx>
#include "json.hpp"


class Batiment
{
public:
    Batiment();
    std::vector<glm::vec3> createBuilding(std::string id);
private:
    std::string id;
};

#endif // BATIMENT_H
