#include <iostream>
#include "Renderer/Primitives.h"
#include "Scene/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"
#include "Scene/Camera.h"
#include "Scene/Lights/Light.h"
#include "Scene/Lights/PointLight.h"
#include "Renderer/DebugShapes.h"

#include "FileManager.h"
#include "Scene/Scene.h"

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "UI/EngineUI.h"
#include "UI/Layers/UISceneTree.h"
#include "UI/Layers/UI_Scene.h"
#include "UI/Layers/UI_ObjectProperties.h"
#include "UI/Layers/UI_CameraProperties.h"

#include "Core/Window.h"
#include "Renderer/FrameQuad.h"
#include "Renderer/Cubemap.h"
#include "Renderer/PostProccess/Bloom.h"

#include "Scene/Skybox.h"


//Temporary global variables, later put them inside engine class
Renderer* renderer_ptr = nullptr;
Window* window_ptr = nullptr;

//move that to engine class
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if(renderer_ptr)
    {
	    renderer_ptr->onWindowResize(width, height);
        window_ptr->setWidth(width); //reset the viewport to the new window size
        window_ptr->setHeight(height);
    }
}

int main(int argc, char* argv[])
{
    //Put window inside application class
    Window window;
    if(!window.init("Render Engine", 1920, 1080))
    {
        std::cout << "Couldn't initialize window!" << std::endl;
        return 1;
    }
	window_ptr = &window; //set the window pointer to the window instance

    glfwSetFramebufferSizeCallback(window.getGLWindow(), frameBufferSizeCallback);

    //Put file manager inside application class
    FileManager fileManager;
    if(!fileManager.init())
    {
        std::cout << "Couldn't initialize File Manager!" << std::endl;
        return 1;
    }

    // create a default directory for the resources
    fileManager.createDirectory("../assets");

    //Move the shader creation to the startup of an Engine class
    //CREATE PBR SHADER
    //Shader shader(FileManager::loadShader("Shaders/Main/vertexShader.glsl"), FileManager::loadShader("Shaders/Main/fragShader.glsl"));
    fileManager.loadShader("PBRShader", "../assets/Shaders/Main/vertexShader.glsl", "../assets/Shaders/Main/fragShader.glsl");
    Shader shader = *Shader::findShader("PBRShader");

    //CREATE BLOOM SHADER
    fileManager.loadShader("BloomShader", "../assets/Shaders/PostProcess/Bloom/bloomVertex.glsl", "../assets/Shaders/PostProcess/Bloom/bloomFrag.glsl");
    Shader bloomShader = *Shader::findShader("BloomShader");

    //CREATE BLUR SHADER
    fileManager.loadShader("BlurShader", "../assets/Shaders/PostProcess/Blur/blurVertex.glsl", "../assets/Shaders/PostProcess/Blur/blurFrag.glsl");
    Shader blurShader = *Shader::findShader("BlurShader");

    //CREATE DEBUG SHADER
    fileManager.loadShader("DebugShader", "../assets/Shaders/Debug/debugVertex.glsl", "../assets/Shaders/Debug/debugFrag.glsl");
    Shader debugShader = *Shader::findShader("DebugShader");

    //CREATE FRAMEQUAD SHADER
    fileManager.loadShader("FramequadShader", "../assets/Shaders/PostProcess/FrameQuad/FrameQuadVertex.glsl", "../assets/Shaders/PostProcess/FrameQuad/FrameQuadFrag.glsl");
    Shader frameQuadShader = *Shader::findShader("FramequadShader");

    //CREATE SKYBOX SHADER
    fileManager.loadShader("SkyboxShader", "../assets/Shaders/Skybox/SkyboxVertex.glsl", "../assets/Shaders/Skybox/SkyboxFrag.glsl");
    Shader skyboxShader = *Shader::findShader("SkyboxShader");

	//CREATE SHADOW SHADER
	fileManager.loadShader("ShadowShader", "../assets/Shaders/Shadow/shadowVertex.glsl", "../assets/Shaders/Shadow/shadowFrag.glsl", "../assets/Shaders/Shadow/shadowGeometry.glsl");
	Shader shadowShader = *Shader::findShader("ShadowShader");

	//CREATE TANGENT SHADER
	fileManager.loadShader("TangentShader", "../assets/Shaders/Debug/tangentVertex.glsl", "../assets/Shaders/Debug/tangentFrag.glsl", "../assets/Shaders/Debug/tangentGeometry.glsl");
	Shader tangentShader = *Shader::findShader("TangentShader");

    Renderer renderer;
    if (!renderer.init(&window))
    {
		std::cout << "Couldn't initialize Renderer!" << std::endl;
		return 1;
    }
    renderer_ptr = &renderer; //set the renderer pointer to the renderer instance

    //Hardcoding scene objects untill I make a factory
    Scene mainScene;
    Scene::activeScene = &mainScene;
    
    Camera mainCamera;
    //mainScene.sceneObjects.push_back(mainCamera_p);
    mainScene.addObject(&mainCamera);
    mainScene.setActiveCamera(&mainCamera);

    Camera otherCamera;
    //mainScene.sceneObjects.push_back(otherCamera_p);
    mainScene.addObject(&otherCamera);

    //Could go to a JSON file that wil be used to load the scene
    std::string cubemapPaths[6] = 
    { 
        "../assets/Skybox/right.jpg",
        "../assets/Skybox/left.jpg",
        "../assets/Skybox/top.jpg",
        "../assets/Skybox/bottom.jpg",
        "../assets/Skybox/front.jpg",
        "../assets/Skybox/back.jpg" 
    };

    //Cubemap testCubeMap(fileManager.loadCubemap(cubemapPaths));
    Skybox skybox(cubemapPaths);
	mainScene.activeSkybox = &skybox;

    mainCamera.setFOV(90.0f);
    mainCamera.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    otherCamera.setFOV(30.0f);

    //Hard coded lights
    for (size_t i = 0; i < 4; i++)
    {
        mainScene.addObject(new PointLight(std::string("PointLight_" + std::to_string(i))));
    }

    //Loading textures and setting materials untill I make it through the UI
    {
        std::vector<std::string> texturePaths = { 
                                            "../assets/AK203/Set1_Base.png",
                                            "../assets/AK203/Set1_ORM.png",
                                            "../assets/AK203/Set1_Normal.png",
                                            "../assets/AK203/Set2_Base.png",
                                            "../assets/AK203/Set2_ORM.png",
                                            "../assets/AK203/Set2_Normal.png",
                                            "../assets/AK203/Set3_Base.png",
                                            "../assets/AK203/Set3_ORM.png",
                                            "../assets/AK203/Set3_Normal.png",
                                            "../assets/AK203/Set4_Base.png",
                                            "../assets/AK203/Set4_ORM.png",
                                            "../assets/AK203/Set4_Normal.png"};

        fileManager.loadTextures(texturePaths);

        // dynamically loaded object
        if (fileManager.loadOBJ("../assets/AK203/AK203.obj"))
        {
            mainScene.materials[2]->setTexture(mainScene.textures[0], 0);
            mainScene.materials[2]->setTexture(mainScene.textures[1], 1);
            mainScene.materials[2]->setTexture(mainScene.textures[2], 2);

            mainScene.materials[0]->setTexture(mainScene.textures[3], 0);
            mainScene.materials[0]->setTexture(mainScene.textures[4], 1);
            mainScene.materials[0]->setTexture(mainScene.textures[5], 2);

            mainScene.materials[3]->setTexture(mainScene.textures[6], 0);
            mainScene.materials[3]->setTexture(mainScene.textures[7], 1);
            mainScene.materials[3]->setTexture(mainScene.textures[8], 2);

            mainScene.materials[1]->setTexture(mainScene.textures[9], 0);
            mainScene.materials[1]->setTexture(mainScene.textures[10], 1);
            mainScene.materials[1]->setTexture(mainScene.textures[11], 2);
        }
    }

    //that will fix the snap after entering camera controll, would need to set some values in the constructors to notuse this line
    mainScene.getActiveCamera()->rotateCam(glm::vec3(0,0,0));

    //Initing ImGUI here
    EngineUI mainUI(&window, &fileManager, &renderer);

    Bloom bloomPP(blurShader, bloomShader);
    bloomPP.setSteps(20);

    //this should be in the scene class
    renderer.debugShapes.drawBox(Point(0.5, 2.2, 3.0), Point(1.4, 1.1, 0.1), Color(0.5, 1, 0));

    //TEST ANOTHER SCENE
	Scene otherScene;
	//Scene::activeScene = &otherScene;

	Camera otherCamera2;
	otherScene.setActiveCamera(&otherCamera2);
	otherCamera2.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
	otherScene.addObject(&otherCamera2);

    otherScene.activeSkybox = &skybox;

    Scene* activeScene = Scene::activeScene;

    double lastFrameTime = glfwGetTime();
    // Loop until the user closes the window, put it inside application/engine class
    while (!window.shouldClose())
    {
		double currentFrameTime = glfwGetTime();
		float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
		lastFrameTime = currentFrameTime;
        activeScene->updateObjects(deltaTime);

		renderer.renderScene(&mainScene, &window); //render the scene with the renderer

		//This can later go into input handling class
        if(mainUI.getSceneLayer().isViewMode())
        {
		    ImVec2 windowSpace = mainUI.getSceneLayer().getWindowSpace();

            activeScene->getActiveCamera()->setAspectRatio(windowSpace.x, windowSpace.y);
            activeScene->getActiveCamera()->updateCamera();

            Camera::CursorData data(windowSpace.x, windowSpace.y, 0, 0, 0, 0);
            activeScene->getActiveCamera()->cameraController(window.getGLWindow(), windowSpace.x, windowSpace.y, deltaTime);
        }

        mainUI.renderUI();
        /* Swap front and back buffers */
        glfwSwapBuffers(window.getGLWindow());

        /* Poll for and process events */
		window.pollEvents();
    }

    renderer.stop();
    fileManager.stop();
    window.stop();

    return 0;
}