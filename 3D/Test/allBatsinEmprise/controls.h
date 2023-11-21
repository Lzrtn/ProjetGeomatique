#ifndef CONTROLS_H
#define CONTROLS_H

#include "camera.h"
#include <GLFW/glfw3.h>
#include "shader.h"

class Controls
{
public:
    Controls(GLFWwindow* window, Camera *camera);

    float getSpeed() const {return speed;};
    void setSpeed(float newSpeed);

    float getMouseSpeed() const {return mouseSpeed;};
    void setMouseSpeed(float newMouseSpeed);
    virtual void update(float deltaTime, Shader *shader)=0;

protected:
    Camera *m_Camera;
    float speed;
    float mouseSpeed;
    int width,height;

    GLFWwindow* m_Window;
};

#endif // CONTROLS_H
