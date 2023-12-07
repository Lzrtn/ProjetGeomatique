#ifndef BATIMENT_H
#define BATIMENT_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include <pqxx/pqxx>
#include "json.hpp"
#include "QVector3D"
#include "building3d.h"

/**
 * @brief The Batiment class represents a building in a 3D environment.
 *
 * This class provides functionality to create a building based on a given ID and IP address.
 */
class Batiment
{
public:
    /**
     * @brief Default constructor for the Batiment class.
     */
    Batiment();

    /**
     * @brief Creates a 3D building based on the provided ID and IP address.
     *
     * @param id The unique identifier of the building.
     * @param ipAddress The IP address associated with the building.
     * @return A vector of QVector3D representing the vertices or positions of the building in 3D space.
     */
    std::vector<QVector3D> createBuilding(std::string id, std::string ipAddress);

private:
    /**
     * @brief The unique identifier of the building.
     */
    std::string id;
};

#endif // BATIMENT_H

