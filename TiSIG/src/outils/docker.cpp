#include "docker.h"

#include <algorithm>


Docker::Docker(std::string pathDockerFile)
{
    // Executor creation
    executor = new Executor();
    // Container creation
    std::string cmdCreateContainerString = "docker start " + pathDockerFile;
    const char * cmdCreateContainer = cmdCreateContainerString.c_str();
    executor->exec(cmdCreateContainer);
    // Ip adress setting
    this->setIpAdress();
}

Docker::~Docker() {
    // Container destruction
    std::string cmdDeleteContainerString = "docker stop database-tisig";
    const char * cmdDeleteContainerChar = cmdDeleteContainerString.c_str();
    executor->exec(cmdDeleteContainerChar);
    // Executor destruction
    if (executor) delete executor;
}

void Docker::setIpAdress() {
    std::string cmdInspectString = "docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' database-tisig";
    const char * cmdInspect = cmdInspectString.c_str();
    auto ip = executor->exec(cmdInspect);
    ip.erase(std::remove(ip.begin(), ip.end(), '\n'), ip.cend());
    this->ipAdress = ip;
}

std::string Docker::getIpAdress()
{
    return ipAdress;
}

