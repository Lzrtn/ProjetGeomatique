#include "pointlight.h"

PointLight::PointLight(glm::vec3& position, glm::vec3& color, float power)
    :position(position), color(color), power(power)
{
}

PointLight::~PointLight() {}

void PointLight::Bind(Shader& shader)
{
    shader.Bind();
    shader.setUniform3fv("lightPosition", position);
    shader.setUniform3fv("lightColor", color);
//    shader.setUniform1f("lightPower", power);

}

void PointLight::Unbind()
{

}
