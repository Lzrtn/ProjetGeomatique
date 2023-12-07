#ifndef BATIMENTS_H
#define BATIMENTS_H
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "json.hpp"
#include "batiment.h"
#include "i_openglcityview.h"


/**
 * @brief The Batiments class represents a collection of 3D buildings in a city environment.
 *
 * This class extends the Object3DStorage interface and provides functionality to manage and render
 * buildings within a specified emprise (area) in a 3D city model.
 */
class Batiments: public Object3DStorage{
public:
    /**
     * @brief Default constructor for the Batiments class.
     */
    Batiments(std::string ip);

    /**
     * @brief Default destructor for the Batiments class.
     */
    ~Batiments();

    /**
     * @brief Retrieves a list of parent IDs based on the specified emprise.
     *
     * @param emprise The emprise (area) for which parent IDs need to be retrieved.
     * @return A vector of strings representing the list of parent IDs.
     */
    std::vector<std::string> getListParentID(const Emprise &emprise);

    /**
     * @brief Retrieves objects within the specified emprise and adds them to the provided collections.
     *
     * @param emprise The emprise (area) for which objects are retrieved.
     * @param new_objects A map to store newly created objects with their IDs.
     * @param show_objects A vector to store IDs of objects to be displayed.
     * @param forget_objects A vector to store IDs of objects to be forgotten.
     */
    void GetObjectsInEmprise(const Emprise &emprise,
                             std::map<int, Object3DFactory*> &new_objects,
                             std::vector<int> &show_objects,
                             std::vector<int> &forget_objects) override;
    bool PickingObjectInfo(
                const QVector3D &/*p1*/,
                const QVector3D &/*p2*/,
                int &/*idObject*/,
            std::map<std::string, std::string> &/*data*/) override{return false;};
    /**
     * @brief Sets the IP address used for communication with external systems.
     *
     * @param ip The IP address to be set.
     */
    void setIP(std::string ip){ipAddress = ip;};


private:
    Batiment bat;
    std::vector<std::string> listParentID;
    std::string ipAddress;
    std::map<int, Object3DFactory*> buildings;
    std::vector<int> indexStorage;
    std::vector<QVector2D> textCoord =  {
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1, 1},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1, 1}
    };

//    std::string textPath = ":/rose des vents.png";
    std::string textPath = "";
};

#endif // BATIMENTS_H
