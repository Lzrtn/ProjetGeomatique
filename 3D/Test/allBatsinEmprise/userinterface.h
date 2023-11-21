#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "pointlight.h"
#include "object.h"
#include "shader.h"

class UserInterface
{
public:
    UserInterface(PointLight* light, Object* o, GLFWwindow* window);
    ~UserInterface();
    void Bind(Shader* shader);
    void Draw();

    float getSpeedCoef() const;
    float getCameraSpeed() const;
    float getSpeed() const;

private:
    PointLight* m_pl;
    Object* m_o;
    float m_speedCoef;
    float m_cameraSpeed;
    float m_speed;
};

#endif // USERINTERFACE_H
