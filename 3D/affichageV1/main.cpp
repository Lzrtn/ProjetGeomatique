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



using json = nlohmann::json;

void translateToBasePoint(std::vector<glm::vec3>& vectorToTranslate, const glm::vec3& basePoint) {
    for (auto& element : vectorToTranslate) {
        element -= basePoint;
    }
}


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
    shader.Bind();
    Shader shaderSoleil(path+"/shaders/soleil.vertexshader", path+"/shaders/soleil.fragmentshader");

    ////////// VERTEXARRAY //////////
    VertexArray va;
    va.Bind();

    ////////// CAMERA //////////
    Camera cam(width, height);


    Object soleil("/home/formation/Documents/OpenGL/TP/models/sphere.obj", path+"/textures/blanc.jpg");
    soleil.position.x = 0;
    soleil.position.y = 0;
    soleil.position.z = 0;


    std::vector<int> id;
    glm::vec3 base;

//    vector<> batiments;

    std::vector<glm::vec3> coordinates;
    std::vector<glm::vec3> coordinatesLe2;
    std::vector<glm::vec2> uvCoordinates;

    std::vector<std::vector<std::vector<double>>> listPolygons;


    try {
        // Définir les paramètres de connexion
        const std::string dbname = "CityGML";
        const std::string user = "postgres";
        const std::string password = "postgres";
        const std::string hostaddr = "127.0.0.1";  // Utilisez "localhost" pour une connexion locale
        const std::string port = "5432";  // Utilisez "5432" pour le port par défaut

        // Construire la chaîne de connexion
        std::string conn_str = "dbname=" + dbname +
                               " user=" + user +
                               " password=" + password +
                               " hostaddr=" + hostaddr +
                               " port=" + port;

        // Se connecter à la base de données
        pqxx::connection conn(conn_str);

        if (conn.is_open()) {
            std::cout << "Connexion réussie à la base de données PostgreSQL." << std::endl;
            pqxx::work txn(conn);
            pqxx::result result = txn.exec("SELECT id, ST_ASGEOJSON(geometry) FROM surface_geometry WHERE parent_id = '364';");
//            pqxx::result parentIDquery = txn.exec("SELECT DISTINCT parent_id FROM surface_geometry WHERE parent_id NOTNULL LIMIT 100");



            for (const auto& row : result) {

                json jsonListPolygons = json::parse(row[1].as<std::string>())["coordinates"];
//                json polygonUVCoordinates = json::parse(row[2].as<std::string>())["coordinates"][0];
                id.push_back(row[0].as<int>());

                for(auto& element: jsonListPolygons)
                {
                    std::vector<std::vector<double>> el = element;
                    listPolygons.push_back(el);
                }

                //                std::cout << listPolygons[0][0][0] << "\n";
                base = glm::vec3(listPolygons[0][0][0], listPolygons[0][0][1], listPolygons[0][0][2]);


           }

            conn.disconnect();
        } else {
            std::cerr << "Impossible de se connecter à la base de données." << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }



    for(auto& polygon: listPolygons)
    {
        std::vector<glm::vec3> listCoords = {};

        //REMPLIR LISTE COORDS
        for(auto& point: polygon){listCoords.push_back(glm::vec3(point[0], point[1], point[2]));}
        translateToBasePoint(listCoords, base);

//        for(auto& element: listCoords)
//        {
//                std::cout << element.x << "\n";
//                std::cout << element.y << "\n";
//                std::cout << element.z << "\n";

//        }

        while(listCoords.size() > 3 )
        {

            std::vector<double> listAngles = {};
            int idxAngleMin = 0;
            for(int sizePolygon = 0; sizePolygon < listCoords.size(); sizePolygon++)
            {

                if(sizePolygon == 0)
                {

                    double xAv = listCoords[listCoords.size()-1][0];
                    double yAv = listCoords[listCoords.size()-1][1];
                    double zAv = listCoords[listCoords.size()-1][2];

                    double x = listCoords[sizePolygon][0];
                    double y = listCoords[sizePolygon][1];
                    double z = listCoords[sizePolygon][2];

                    double xAp = listCoords[sizePolygon+1][0];
                    double yAp = listCoords[sizePolygon+1][1];
                    double zAp = listCoords[sizePolygon+1][2];

                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
                    if(angle < M_PI && angle > 0)
                    {
                        listAngles.push_back(angle);
                    }

                }

                if(sizePolygon > 0 && sizePolygon < listCoords.size()-2)
                {
                    double xAv = listCoords[sizePolygon-1][0];
                    double yAv = listCoords[sizePolygon-1][1];
                    double zAv = listCoords[sizePolygon-1][2];

                    double x = listCoords[sizePolygon][0];
                    double y = listCoords[sizePolygon][1];
                    double z = listCoords[sizePolygon][2];

                    double xAp = listCoords[sizePolygon+1][0];
                    double yAp = listCoords[sizePolygon+1][1];
                    double zAp = listCoords[sizePolygon+1][2];

                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
                    if(angle < M_PI && angle > 0)
                    {
                        listAngles.push_back(angle);
                    }

                }


                if(sizePolygon == listCoords.size()-1)
                {
                    double xAv = listCoords[sizePolygon-1][0];
                    double yAv = listCoords[sizePolygon-1][1];
                    double zAv = listCoords[sizePolygon-1][2];

                    double x = listCoords[sizePolygon][0];
                    double y = listCoords[sizePolygon][1];
                    double z = listCoords[sizePolygon][2];

                    double xAp = listCoords[0][0];
                    double yAp = listCoords[0][1];
                    double zAp = listCoords[0][2];

                    double angle = acos(((xAv - x)*(xAp - x) + (yAv - y)*(yAp - y)) / (sqrt(pow(xAv - x, 2) + pow(yAv - y, 2)) * sqrt(pow(xAp - x, 2) + pow(yAp - y, 2))));
                    if(angle < M_PI && angle > 0)
                    {
                        listAngles.push_back(angle);
                    }

                }

                    auto it = std::find(listAngles.begin(), listAngles.end(), *min_element(listAngles.begin(), listAngles.end()));
                    if (it != listAngles.end()) {
                        idxAngleMin = std::distance(listAngles.begin(), it);
                    }
                    std::cout << "---" << "\n";

                    if(idxAngleMin == 0)
                    {
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][1], listCoords[idxAngleMin][2]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin+1][0], listCoords[idxAngleMin+1][1], listCoords[idxAngleMin+1][2]));
                        coordinates.push_back(glm::vec3(listCoords[listCoords.size()-1][0], listCoords[listCoords.size()-1][1], listCoords[listCoords.size()-1][2]));

                    }

                    if(idxAngleMin > 0 && idxAngleMin < listCoords.size()-2)
                    {
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][1], listCoords[idxAngleMin][2]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin+1][0], listCoords[idxAngleMin+1][1], listCoords[idxAngleMin+1][2]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin-1][0], listCoords[idxAngleMin-1][1], listCoords[idxAngleMin-1][2]));
                    }

                    if(sizePolygon == listCoords.size()-1)
                    {
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin][0], listCoords[idxAngleMin][1], listCoords[idxAngleMin][2]));
                        coordinates.push_back(glm::vec3(listCoords[0][0], listCoords[0][1], listCoords[0][2]));
                        coordinates.push_back(glm::vec3(listCoords[idxAngleMin-1][0], listCoords[idxAngleMin-1][1], listCoords[idxAngleMin-1][2]));
                    }

                    listCoords.erase(listCoords.begin() + idxAngleMin);

            }

//            for(auto& element: listCoords)
//            {
//                    std::cout << element.x << "\n";
//                    std::cout << element.y << "\n";
//                    std::cout << element.z << "\n";

//            }



//            std::cout << "---------" << "\n";

        }



    }

    //    for(auto& element : listPolygons)
    //    {
    //        for(auto& point: element)
    //        {
//            for(auto& coord: point)
//            {
//                std::cout << coord << "\n";
//            }

//        }
//    }


    vector<glm::vec3> g_coordinates = {
        glm::vec3(0, 0, 0),
    };

    vector<glm::vec2> g_uvCoordinates = {
        glm::vec2( 0,0)
    };

    Object o(coordinates, g_uvCoordinates, path+"/textures/sun.jpg");

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
        renderer.Clear();
        shader.setUniformMat4f("model", o.getModelMatrix());
        renderer.Draw(va, o, shader);

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
