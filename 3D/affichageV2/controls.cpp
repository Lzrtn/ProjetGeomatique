#include "controls.h"

Controls::Controls(GLFWwindow* window, Camera *camera):m_Camera(camera),m_Window(window), speed(6), mouseSpeed(0.05)
{
    glfwGetWindowSize(window, &width, &height);
}

void Controls::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Controls::setMouseSpeed(float newMouseSpeed)
{
    mouseSpeed = newMouseSpeed;
}

