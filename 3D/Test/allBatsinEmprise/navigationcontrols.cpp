#include "navigationcontrols.h"
#include "imgui/imgui.h"

NavigationControls::NavigationControls(GLFWwindow *window, Camera *camera, UserInterface &interface)
    :Controls(window, camera), lastPosCursor(-1,-1), interface(&interface)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
     mouseSpeed = 0.2;
     speed = 100;

}

void NavigationControls::update(float deltaTime, Shader *shader)
{

    interface->Draw();
    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse){

        int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
        double xpos, ypos;
        glfwGetCursorPos(m_Window, &xpos, &ypos);
        if(state == GLFW_PRESS){
            if (lastPosCursor.x!=-1){
                m_Camera->horizontalAngle -= mouseSpeed * deltaTime * float( xpos-lastPosCursor.x );
                m_Camera->verticalAngle  -= mouseSpeed * deltaTime * float( ypos-lastPosCursor.y );
            }
            lastPosCursor.x = xpos;
            lastPosCursor.y = ypos;
        }
        else{
           lastPosCursor = glm::vec2(-1,-1);
        }

        glm::vec3 direction(
            cos(m_Camera->verticalAngle) * sin(m_Camera->horizontalAngle),
            sin(m_Camera->verticalAngle),
            cos(m_Camera->verticalAngle) * cos(m_Camera->horizontalAngle)
        );

        // Right vector
        glm::vec3 right = glm::vec3(
            sin(m_Camera->horizontalAngle - 3.14f/2.0f),
            0,
            cos(m_Camera->horizontalAngle - 3.14f/2.0f)
        );

        // Up vector : perpendicular to both direction and right
        glm::vec3 up = glm::cross( right, direction );


        // Move forward
        if (glfwGetKey(m_Window, GLFW_KEY_UP ) == GLFW_PRESS){
            m_Camera->position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(m_Window, GLFW_KEY_DOWN ) == GLFW_PRESS){
            m_Camera->position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(m_Window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
            m_Camera->position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(m_Window, GLFW_KEY_LEFT ) == GLFW_PRESS){
            m_Camera->position -= right * deltaTime * speed;
        }
        // Move forward
        if (glfwGetKey(m_Window, GLFW_KEY_W ) == GLFW_PRESS){
            m_Camera->position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(m_Window, GLFW_KEY_S ) == GLFW_PRESS){
            m_Camera->position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(m_Window, GLFW_KEY_D ) == GLFW_PRESS){
            m_Camera->position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(m_Window, GLFW_KEY_A ) == GLFW_PRESS){
            m_Camera->position -= right * deltaTime * speed;
        }
        // go up
        if (glfwGetKey(m_Window, GLFW_KEY_SPACE ) == GLFW_PRESS){
            m_Camera->position += up * deltaTime * speed;
        }
        // go down
        if (glfwGetKey(m_Window, GLFW_KEY_Q ) == GLFW_PRESS){
            m_Camera->position -= up * deltaTime * speed;
        }
    }

}
