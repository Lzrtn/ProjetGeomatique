#ifndef DOCKER_H
#define DOCKER_H

#include "executor.h"

class Docker
{
public:
    /**
     * @brief Read a docker-compose.yml, specified in the parameters and create a container
     * @param pathDockerCompose : Relative (or absolute) path of the docker compose file
     */
    Docker(std::string pathDockerCompose = ".");

    ~Docker();

    /**
     * @brief Return the ip Adress of the docker container
     * @return ip adress
     */
    std::string getIpAdress();

private:

    /**
     * @brief set the ipAdress attribute by executing a command
     */
    void setIpAdress();


    Executor * executor;
    std::string ipAdress;
};

#endif // DOCKER_H
