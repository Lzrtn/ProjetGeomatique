#ifndef FPSCONTROLS_H
#define FPSCONTROLS_H

#include "controls.h"

class FPSControls : public Controls
{
public:
    FPSControls(GLFWwindow* window, Camera *camera);
    void update(float deltaTime, Shader* shader);
};

#endif // FPSCONTROLS_H
