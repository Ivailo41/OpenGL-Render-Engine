#include <iostream>
#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "ObjectArray.h"
#include "UI/EngineUI.h"

#include "FileManager.h"
#include "Editor/Scene.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include "Includes/imGuiInclude.h"

#include "UI/EngineUI.h"
#include "UI/Layers/UIScene.h"
#include "UI/Layers/UI_ObjectProperties/UI_ObjectProperties.h"
#include "UI/Layers/UI_CameraProperties/UI_CameraProperties.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1024, "Render Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int windowX, windowY;
    glfwGetWindowSize(window, &windowX, &windowY);

    EngineUI engineUI(window);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW ERROR" << std::endl;
        return -1;
    }
    //End of initialization

    Scene mainScene;
    Scene::activeScene = &mainScene;

    Object dummyObject;
    BaseObject* dummy = &dummyObject;

    Camera mainCamera;
    mainScene.sceneObjects.addObject(mainCamera);
    Camera otherCamera;
    mainScene.sceneObjects.addObject(otherCamera);

    mainCamera.setFOV(90.0f);
    mainCamera.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    otherCamera.setFOV(30.0f);

    Camera* currentCamera = &mainCamera;

    //Fix allocation of memory
    std::vector<Light*> lights;
    for (size_t i = 0; i < 4; i++)
    {
        PointLight light(std::string("PointLight_" + std::to_string(i)));
        lights.push_back(new PointLight(std::string("PointLight_" + std::to_string(i)))); //ERROR HERE READING DELETED MEMOEY WHEN DESTRUCTOR IS CALLED
        mainScene.sceneObjects.addObject(lights[i]);
    }
    lights[0]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    lights[1]->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    lights[2]->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    lights[3]->setPosition(glm::vec3(-10.0f, 0.0f, 0.0f));

    //CREATE SHADER
    Shader shader("Shaders/Main/vertexShader.glsl", "Shaders/Main/fragShader.glsl");
    unsigned shaderProgram = Shader::shaders[0];

    /*Texture::loadTexture("resources/Set1_base.png");
    Texture::loadTexture("resources/Set1_Normal.png");
    Texture::loadTexture("resources/Set2_base.png");
    Texture::loadTexture("resources/Set2_Normal.png");
    Texture::loadTexture("resources/Set3_base.png");
    Texture::loadTexture("resources/Set3_Normal.png");
    Texture::loadTexture("resources/Set4_base.png");
    Texture::loadTexture("resources/Set4_Normal.png");
    Texture::loadTexture("resources/Set1_ORM.png");
    Texture::loadTexture("resources/Set2_ORM.png");
    Texture::loadTexture("resources/Set3_ORM.png");
    Texture::loadTexture("resources/Set4_ORM.png");*/

    Texture::loadTexture("resources/Brick_Base.jpg");
    Texture::loadTexture("resources/Brick_Normal.jpg");

    BaseObject* cube = mainScene.loadObject("resources/sphere.obj"); // dynamically allocated

    BaseObject* monkey = mainScene.loadObject("resources/monkey.obj");
    monkey->setPosition(glm::vec3(0.0f, 0.0f, 2.0f));

    /*Material::getMaterial(0)->setTexture(Texture::textures[2], 0);
    Material::getMaterial(0)->setTexture(Texture::textures[9], 1); 
    Material::getMaterial(0)->setTexture(Texture::textures[3], 2); 
    Material::getMaterial(2)->setTexture(Texture::textures[0], 0); 
    Material::getMaterial(2)->setTexture(Texture::textures[8], 1);
    Material::getMaterial(2)->setTexture(Texture::textures[1], 2);
    Material::getMaterial(3)->setTexture(Texture::textures[4], 0);
    Material::getMaterial(3)->setTexture(Texture::textures[10],1);
    Material::getMaterial(3)->setTexture(Texture::textures[5], 2); 
    Material::getMaterial(1)->setTexture(Texture::textures[6], 0); 
    Material::getMaterial(1)->setTexture(Texture::textures[11],1);
    Material::getMaterial(1)->setTexture(Texture::textures[7], 2);*/

    Material::getMaterial(0)->setTexture(Texture::textures[0], 0);
    Material::getMaterial(0)->setTexture(Texture::textures[1], 2);

    Material::getMaterial(1)->setTexture(Texture::textures[0], 0);
    Material::getMaterial(1)->setTexture(Texture::textures[1], 2);

    mainScene.setSelectedObject(cube);

    glUseProgram(Shader::shaders[0]);

    //temp
    UIScene uiScene;
    UI_ObjectProperties uiObjectProperties;
    UI_CameraProperties uiCameraProperties(currentCamera);

    EngineUI mainUI(window);
    mainUI.addUILayer(&uiScene);
    mainUI.addUILayer(&uiObjectProperties);
    mainUI.addUILayer(&uiCameraProperties);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_FRONT);

        currentCamera->cameraController(window, windowX, windowY);

        //PASS LIGHTS TO SHADER
        for(unsigned i = 0; i < lights.size(); i++)
        {
            lights[i]->sendToShader(Shader::shaders[0], i);
        }

        mainScene.drawObjects();

        //cube->draw();

        mainUI.renderUI();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}