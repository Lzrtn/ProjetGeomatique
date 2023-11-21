#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "shader.h"

class Camera
{
public:
    Camera(float width, float height);
    void computeMatrices(float width, float height);

    glm::vec3 position;

    float horizontalAngle;

    float verticalAngle;

    void setFoV(float newFoV);

    const glm::mat4 &getViewMatrix() const;

    const glm::mat4 &getProjectionMatrix() const;

    void Bind(Shader *shader);

private:
    float FoV;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

#endif // CAMERA_H
