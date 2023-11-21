#include "userinterface.h"
#include "./imgui/imgui.h"
#include "./imgui/backends/imgui_impl_glfw.h"
#include "./imgui/backends/imgui_impl_opengl3.h"

UserInterface::UserInterface(PointLight* light, Object* o, GLFWwindow* window)
    :m_pl(light), m_o(o), m_speedCoef(1.0f), m_cameraSpeed(0.1f), m_speed(20.0f)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");


}

UserInterface::~UserInterface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

float UserInterface::getSpeedCoef() const
{
    return m_speedCoef;
}

float UserInterface::getCameraSpeed() const
{
    return m_cameraSpeed;
}


float UserInterface::getSpeed() const
{
    return m_speed;
}



void UserInterface::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    ImGui::Begin("My name is window, ImGUI window");
    ImGui::Text("Hello there adventurer!");
    ImGui::SliderFloat("Vitesse", &m_speedCoef, 0.0f, 20.0f);
    ImGui::SliderFloat("CameraSpeed", &m_cameraSpeed, 0.0f, 0.5f);
    ImGui::SliderFloat("MoveSpeed", &m_speed, 0.0f, 500.0f);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
