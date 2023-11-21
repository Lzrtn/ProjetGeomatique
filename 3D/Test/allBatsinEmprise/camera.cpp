#include "camera.h"
#include "renderer.h"

Camera::Camera(float width, float height):position(0,0,0), horizontalAngle(3.14), verticalAngle(0), FoV(90)
{
    computeMatrices(width, height);
}

void Camera::computeMatrices(float width, float height)
{

    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    // Up vector : perpendicular to both direction and right
    glm::vec3 up = glm::cross( right, direction );


    // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    projectionMatrix = glm::perspective(glm::radians(FoV), width/height, 0.1f, 1000.0f);
    // Camera matrix
    viewMatrix       = glm::lookAt(
        position,           // Camera is here
        position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

}

void Camera::setFoV(float newFoV)
{
    FoV = newFoV;
}

const glm::mat4 &Camera::getViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4 &Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}

void Camera::Bind(Shader *shader)
{
    shader->setUniform3fv("camPosition", position);
}


