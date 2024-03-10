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
#include "UI/Layers/UISceneTree.h"
#include "UI/Layers/UI_Scene/UI_Scene.h"
#include "UI/Layers/UI_ObjectProperties/UI_ObjectProperties.h"
#include "UI/Layers/UI_CameraProperties/UI_CameraProperties.h"

int main(void)
{
    GLFWwindow* window;

    // create a default directory for the resources
    FileManager::createDirectory("resources");
    

    /* Initialize the library */
    if (!glfwInit())
        return -1;
 

    window = glfwCreateWindow(800, 600, "Render Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    // Getting the primary monitor and setting the window to be fullscreen
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int monitorX, monitorY;
        glfwGetMonitorPos(monitor, &monitorX, &monitorY);
        int monitorWidth = mode->width;
        int monitorHeight = mode->height;

        glfwSetWindowPos(window, monitorX, monitorY);
        glfwSetWindowSize(window, monitorWidth, monitorHeight);

        std::cout << "Monitor: " << monitorWidth << ", " << monitorHeight << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, nullptr);

    int windowX, windowY;
    glfwGetWindowSize(window, &windowX, &windowY);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW ERROR" << std::endl;
        return -1;
    }
    //End of initialization

    //Hardcoding scene objects untill I make a factory
    Scene mainScene;
    Scene::activeScene = &mainScene;

    Object dummyObject;
    BaseObject* dummy = &dummyObject;

    Camera* mainCamera = new Camera;
    mainScene.sceneObjects.push_back(mainCamera);
    mainScene.setActiveCamera(mainCamera);

    Camera* otherCamera = new Camera;
    mainScene.sceneObjects.push_back(otherCamera);

    mainCamera->setFOV(90.0f);
    mainCamera->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    otherCamera->setFOV(30.0f);

    //Hard coded lights, later do an object factory that passes the created lights to the scene's array of lights
    std::vector<Light*> lights;
    for (size_t i = 0; i < 4; i++)
    {
        lights.push_back(new PointLight(std::string("PointLight_" + std::to_string(i))));
        mainScene.sceneObjects.push_back(lights[i]);
    }
    lights[0]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    lights[1]->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    lights[2]->setPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    lights[3]->setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));

    //CREATE SHADER
    Shader shader("Shaders/Main/vertexShader.glsl", "Shaders/Main/fragShader.glsl");
    unsigned shaderProgram = Shader::shaders[0];

    //Loading textures and setting materials untill I make it through the UI

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

    /*Texture::loadTexture("resources/Brick_Base.jpg");
    Texture::loadTexture("resources/Brick_Normal.jpg");
    Texture::loadTexture("resources/Marble_ORM5.jpg");
    */

    Texture::loadTexture("resources/Marble_Albedo.jpg");
    Texture::loadTexture("resources/Marble_Normal.jpg");
    Texture::loadTexture("resources/Marble_ORM.png");
       
    // dynamically load object
    try {
        mainScene.loadObject("resources/cube.obj");
    }
    catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

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

    try {
        Material::getMaterial(0)->setTexture(Texture::textures[0], 0);
        Material::getMaterial(0)->setTexture(Texture::textures[1], 2);
        Material::getMaterial(0)->setTexture(Texture::textures[2], 1);
    }
    catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    mainScene.setSelectedObject(nullptr);

    glUseProgram(Shader::shaders[0]);

    EngineUI mainUI(window);

    //temp - frameBuffer
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    unsigned fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    unsigned int rbo;

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //temp UI creation
    UISceneTree uiScene;
    UI_Scene uiSceneLayer(window, texture);
    UI_ObjectProperties uiObjectProperties;
    UI_CameraProperties uiCameraProperties(mainScene.getActiveCamera());
    mainUI.addUILayer(&uiScene);
    mainUI.addUILayer(&uiObjectProperties);
    mainUI.addUILayer(&uiCameraProperties);
    mainUI.addUILayer(&uiSceneLayer);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_FRONT);

        //TEMPORARY FIX FOR THE LIGHTING BECAUSE I WAS NOT PASSING THE CAMERA POSITION INTO THE SHADER AND I WANT TO KMS
        unsigned int camPos = glGetUniformLocation(Shader::shaders[0], "camPos");
        glUniform3f(camPos, mainCamera->getPosition().x, mainCamera->getPosition().y, mainCamera->getPosition().z);

        //PASS LIGHTS TO SHADER
        for(unsigned i = 0; i < lights.size(); i++)
        {
            lights[i]->sendToShader(Shader::shaders[0], i);
        }

        mainScene.drawObjects();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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