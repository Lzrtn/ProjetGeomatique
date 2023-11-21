#ifndef NAVIGATIONCONTROLS_H
#define NAVIGATIONCONTROLS_H

#include "controls.h"
#include "userinterface.h"

class NavigationControls : public Controls
{
public:
    NavigationControls(GLFWwindow* window, Camera *camera, UserInterface &interface);
    void update(float deltaTime, Shader *shader);

private:
    glm::vec2 lastPosCursor;
    UserInterface* interface;

};

#endif // NAVIGATIONCONTROLS_H
