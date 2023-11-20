#include <iostream>
#include <pqxx/pqxx>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "math.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "renderer.h"
#include "camera.h"
#include "navigationcontrols.h"
#include "objmodel.h"
#include "pointlight.h"
#include "delaunator.hpp"
#include"imgui/imgui.h"
#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"
#include "json.hpp"
#include "batiments.h"
#include "batiment.h"

using json = nlohmann::json;
using namespace std;

int main()
{

    string path = "/home/formation/Documents/OpenGL/TP";

    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); //antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //version core

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);


    //définition de la taille de la fenêtre
    int width=600;
    int height=600;

    //On récupère les dimensions de l'écran pour créer la fenètre
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(primary,nullptr,nullptr, &width, &height);

    //Enfin on crée la fenêtre
    GLFWwindow* window = glfwCreateWindow(width,height,"OpenGL_API",NULL,NULL);
    glfwSwapInterval(1);
    //On vérifie que l'initialisation a bien marché
    if (window==NULL){
        fprintf(stderr, "Erreur lors de la création de la fénêtre\n");
        glfwTerminate();
        return -1;
    }

    //Enfin on définit la fenêtre créée comme la fenêtre sur laquelle on va dessiner
    glfwMakeContextCurrent(window);

    //Initialisation de GLEW
    glewExperimental=true;
    if (glewInit() != GLEW_OK){
        fprintf(stderr, "Erreur lors de l'initialisation de GLEW\n");
        return -1;
    }

    ////////// RENDERER //////////
    Renderer renderer;

    ////////// SHADERS //////////
    Shader shader(path+"/shaders/SimpleVertexShader.vertexshader", path+"/shaders/SimpleFragmentShader.fragmentshader");
    //shader.Bind();

    ////////// VERTEXARRAY //////////
    VertexArray va;
    va.Bind();

    ////////// CAMERA //////////
    Camera cam(width, height);

    vector<glm::vec2> g_uvCoordinates = {
        glm::vec2( 0,0)
    };

     Object soleil("/home/formation/Documents/OpenGL/TP/models/sphere.obj", path+"/textures/uranus.jpg");

    Batiments bats;
    std::vector<std::string> listParentID = bats.getListParentID();
    std::vector<std::vector<glm::vec3>> listBats;

    for(auto& id: listParentID)
    {
        Batiment bat;
        listBats.push_back(bat.createBuilding(id));

    }

    std::vector<Object*> listObjBats;
    for(auto& bat: listBats)
    {
//            Object o = new Object(bat, g_uvCoordinates, path+"/textures/uranus.jpg");
//            listObjBats.push_back(&o);
        listObjBats.emplace_back(bat, g_uvCoordinates, path + "/textures/uranus.jpg");
    }


//    for(auto& element: listBats)
//    {
//        for(auto& el: element)
//        {
//            std::cout << el.x << "\n";
//        }

//         std::cout << "------" << "\n";

//    }


    ////////// LUMIERE //////////
    glm::vec3 lightPosition1(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    float lightPower = 1.0f;
    PointLight light(lightPosition1, lightColor, lightPower);
    light.Bind(shader);

    ////////// INTERFACE ET CONTROLES //////////
    UserInterface interface(&light, &soleil, window);
    NavigationControls controls(window, &cam, interface);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    float lastTime = glfwGetTime();
    float currentTime, deltaTime;


    ////////// BOUCLE DE RENDU //////////
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)){

        renderer.Clear();
        currentTime = glfwGetTime();
        deltaTime = currentTime-lastTime;
        lastTime = currentTime;

        cam.computeMatrices(width, height);

        ////////// LUMIERE //////////
        glm::vec3 lightPosition1(0.0f, 0.0f, 0.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        float lightPower = 1.0f;
        PointLight light(lightPosition1, lightColor, lightPower);
        light.Bind(shader);

        ////////// ENVOYER LES MATRICES VIEW ET PROJECTION AUX SHADERS //////////
        shader.setUniformMat4f("view", cam.getViewMatrix());
        shader.setUniformMat4f("projection", cam.getProjectionMatrix());

        ////////// VIDER LES BUFFERS //////////


        for(auto& objBat: listObjBats)
        {
            shader.setUniformMat4f("model", objBat.getModelMatrix());
            renderer.Draw(va, objBat, shader);

        }


        ////////// UPDATE LES CONTROLES //////////
        controls.update(deltaTime, &shader);
        controls.setMouseSpeed(interface.getCameraSpeed());
        controls.setSpeed(interface.getSpeed());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
