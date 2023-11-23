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
#include <map>
#include <bits/stdc++.h>


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

    std::map<std::string, Object*> objMap;

    for (std::string idx: listParentID) {

        Batiment bat;
        Object* o = new Object(bat.createBuilding(idx), g_uvCoordinates, path+"/textures/uranus.jpg");
        objMap.insert({idx, o});
    }

//    for(auto& element: objMap)
//            {
//                std::cout << element.first << ", " << element.second <<"\n";
//            }




//    std::vector<Object*> listObjBats;
//    Object o1(listBats[0], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o2(listBats[1], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o3(listBats[2], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o4(listBats[3], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o5(listBats[4], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o6(listBats[5], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o7(listBats[6], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o8(listBats[7], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o9(listBats[8], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o10(listBats[9], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o11(listBats[10], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o12(listBats[11], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o13(listBats[12], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o14(listBats[13], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o15(listBats[14], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o16(listBats[15], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o17(listBats[16], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o18(listBats[17], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o19(listBats[18], g_uvCoordinates, path+"/textures/uranus.jpg");
//    Object o20(listBats[19], g_uvCoordinates, path+"/textures/uranus.jpg");


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
        ///

        for (const auto& pair : objMap) {
                Object* objectPtr = pair.second;
                shader.setUniformMat4f("model", objectPtr->getModelMatrix());
                renderer.Draw(va, *objectPtr, shader);

        }


        //        for(auto& element: objMap)
        //        {
        //            shader.setUniformMat4f("model", element.second->getModelMatrix());
        //            renderer.Draw(va, *element.second, shader);

        //        }

        //        for(auto bat : listObjBats)
        //        {
        //            std::cout << "--------" << "\n";
        //            shader.setUniformMat4f("model", bat->getModelMatrix());
        //            renderer.Draw(va, *bat, shader);
        //        }

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
