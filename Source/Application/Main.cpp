#include <iostream>
#include "Primitives.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "ObjectArray.h"
#include "Debug/DebugShapes.h"

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

#include "GL_Objects/Window.h"
#include "GL_Objects/FrameQuad.h"
#include "GL_Objects/Cubemap.h"
#include "PostProccess/Bloom.h"

#include "Objects/Skybox.h"

int main(void)
{
    //GLFWwindow* window;
    Window* window = Window::getInstance("Render Engine", 1920, 1080);

    FileManager fileManager;
    fileManager.init();

    // create a default directory for the resources
    fileManager.createDirectory("resources");

    //CREATE SHADER
    //Shader shader(FileManager::loadShader("Shaders/Main/vertexShader.glsl"), FileManager::loadShader("Shaders/Main/fragShader.glsl"));
    fileManager.loadShader("PBRShader", "Shaders/Main/vertexShader.glsl", "Shaders/Main/fragShader.glsl");
    Shader& shader = Shader::shadersList.find("PBRShader").operator*().second;

    //CREATE BLOOM SHADER
    Shader bloomShader(fileManager.loadShader("Shaders/PostProcess/Bloom/bloomVertex.glsl"), fileManager.loadShader("Shaders/PostProcess/Bloom/bloomFrag.glsl"));

    //CREATE BLUR SHADER
    Shader blurShader(fileManager.loadShader("Shaders/PostProcess/Blur/blurVertex.glsl"), fileManager.loadShader("Shaders/PostProcess/Blur/blurFrag.glsl"));

    //CREATE DEBUG SHADER
    Shader debugShader(fileManager.loadShader("Shaders/Debug/debugVertex.glsl"), fileManager.loadShader("Shaders/Debug/debugFrag.glsl"));

    //CREATE FRAMEQUAD SHADER
    Shader frameQuadShader(fileManager.loadShader("Shaders/PostProcess/FrameQuad/FrameQuadVertex.glsl"), fileManager.loadShader("Shaders/PostProcess/FrameQuad/FrameQuadFrag.glsl"));

    //CREATE SKYBOX SHADER
    Shader skyboxShader(fileManager.loadShader("Shaders/Skybox/SkyboxVertex.glsl"), fileManager.loadShader("Shaders/Skybox/SkyboxFrag.glsl"));

    //Hardcoding scene objects untill I make a factory
    Scene mainScene;
    Scene::activeScene = &mainScene;

    Object dummyObject;
    BaseObject* dummy = &dummyObject;

    Camera mainCamera;
    Camera* mainCamera_p = &mainCamera;
    mainScene.sceneObjects.push_back(mainCamera_p);
    mainScene.setActiveCamera(mainCamera_p);

    Camera otherCamera;
    Camera* otherCamera_p = &otherCamera;
    mainScene.sceneObjects.push_back(otherCamera_p);

    //Could go to a JSON file that wil be used to load the scene
    std::string cubemapPaths[6] = 
    { 
        "resources/Skybox/right.jpg",
        "resources/Skybox/left.jpg",
        "resources/Skybox/top.jpg",
        "resources/Skybox/bottom.jpg",
        "resources/Skybox/front.jpg",
        "resources/Skybox/back.jpg" 
    };

    Cubemap testCubeMap(fileManager.loadCubemap(cubemapPaths));
    Skybox skybox(&testCubeMap, &skyboxShader);

    mainCamera_p->setFOV(90.0f);
    mainCamera_p->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    otherCamera_p->setFOV(30.0f);

    //Hard coded lights, later do an object factory that passes the created lights to the scene's array of lights
    std::vector<Light*> lights;
    for (size_t i = 0; i < 4; i++)
    {
        lights.push_back(new PointLight(std::string("PointLight_" + std::to_string(i))));
        mainScene.sceneObjects.push_back(lights[i]);
    }
    lights[0]->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    lights[0]->setIntensity(100);

    lights[1]->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    lights[1]->setIntensity(100);

    lights[2]->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
    lights[2]->setIntensity(100);

    lights[3]->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
    lights[3]->setIntensity(100);

    lights[0]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));
    lights[1]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));
    lights[2]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));
    lights[3]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));

    //Loading textures and setting materials untill I make it through the UI
    {

//Testing multithread loading textures versus one thread
#define ASYNC 1

#if ASYNC
        std::vector<std::string> texturePaths = { 
                                            "resources/AK203/Set1_Base.png",
                                            "resources/AK203/Set1_ORM.png",
                                            "resources/AK203/Set1_Normal.png",
                                            "resources/AK203/Set2_Base.png",
                                            "resources/AK203/Set2_ORM.png",
                                            "resources/AK203/Set2_Normal.png",
                                            "resources/AK203/Set3_Base.png",
                                            "resources/AK203/Set3_ORM.png",
                                            "resources/AK203/Set3_Normal.png",
                                            "resources/AK203/Set4_Base.png",
                                            "resources/AK203/Set4_ORM.png",
                                            "resources/AK203/Set4_Normal.png" };

        std::vector<Texture> testTextures = fileManager.loadTextures(texturePaths);

#else //Single thread texture loading

        Texture set1Base2(FileManager::loadTexture("resources/AK203/Set1_Base.png" ));
        Texture set1ORM2(FileManager::loadTexture("resources/AK203/Set1_ORM.png"));
        Texture set1Normal2(FileManager::loadTexture("resources/AK203/Set1_Normal.png"));

        Texture set2Base2(FileManager::loadTexture("resources/AK203/Set2_Base.png"));
        Texture set2ORM2(FileManager::loadTexture("resources/AK203/Set2_ORM.png"));
        Texture set2Normal2(FileManager::loadTexture("resources/AK203/Set2_Normal.png"));

        Texture set3Base2(FileManager::loadTexture("resources/AK203/Set3_Base.png"));
        Texture set3ORM2(FileManager::loadTexture("resources/AK203/Set3_ORM.png"));
        Texture set3Normal2(FileManager::loadTexture("resources/AK203/Set3_Normal.png"));

        Texture set4Base2(FileManager::loadTexture("resources/AK203/Set4_Base.png"));
        Texture set4ORM2(FileManager::loadTexture("resources/AK203/Set4_ORM.png"));
        Texture set4Normal2(FileManager::loadTexture("resources/AK203/Set4_Normal.png"));


#endif


        /*Texture::loadTexture("resources/Brick_Base.jpg");
        Texture::loadTexture("resources/Brick_Normal.jpg");
        Texture::loadTexture("resources/Marble_ORM5.jpg");*/
        

        /*Texture::loadTexture("resources/Marble_Albedo.jpg");
        Texture::loadTexture("resources/Marble_Normal.jpg");
        Texture::loadTexture("resources/Marble_ORM.png");*/

        // dynamically loaded object
        if (!mainScene.addObject(fileManager.readOBJ("resources/AK203/AK203.obj")))
        {
            std::cout << "Could not load object" << std::endl;;
        }

        //setting the materials of the AK203
        try
        {
            Material::getMaterial(2)->setTexture(testTextures[0], 0);
            Material::getMaterial(2)->setTexture(testTextures[1], 1);
            Material::getMaterial(2)->setTexture(testTextures[2], 2);

            Material::getMaterial(0)->setTexture(testTextures[3], 0);
            Material::getMaterial(0)->setTexture(testTextures[4], 1);
            Material::getMaterial(0)->setTexture(testTextures[5], 2);

            Material::getMaterial(3)->setTexture(testTextures[6], 0);
            Material::getMaterial(3)->setTexture(testTextures[7], 1);
            Material::getMaterial(3)->setTexture(testTextures[8], 2);

            Material::getMaterial(1)->setTexture(testTextures[9], 0);
            Material::getMaterial(1)->setTexture(testTextures[10], 1);
            Material::getMaterial(1)->setTexture(testTextures[11], 2);
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

        /*try {
            Material::getMaterial(0)->setTexture(Texture::textures[0], 0);
            Material::getMaterial(0)->setTexture(Texture::textures[1], 2);
            Material::getMaterial(0)->setTexture(Texture::textures[2], 1);
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }*/
    }


    mainScene.setSelectedObject(nullptr);
    mainScene.getActiveCamera()->rotateCam(glm::vec3(0,0,0));

    //Initing ImGUI here
    EngineUI mainUI(window);
    const UI_Settings& settingsLayer = mainUI.getSettingsLayer();

    FrameQuad::initFrameQuad(&frameQuadShader);

    FrameBuffer firstPassBuffer(2, true);
    firstPassBuffer.genFrameBuffer(1920, 1080);

    Bloom bloomPP(blurShader, bloomShader);
    bloomPP.setSteps(20);

    DebugShapes debugShapes;
    DebugShapes debugShapes2;

    debugShapes.drawBox(Point(0, 0, 0), Point(1, 1, 1), Color(1,0,0));
    debugShapes.drawBox(Point(0.2, 0.2, 0.2), Point(0.4, 0.4, 0.4), Color(1,1,0));
    debugShapes.drawLine(Point(0.2, 0.2, 0.2), Point(0.4, 0.4, 0.4), Color(1,0,0));
    debugShapes.drawBox(Point(1.2, 0.2, 0.2), Point(0.4, 0.4, 0.0), Color(0,0,1));
    debugShapes.drawBox(Point(0.5, 0.2, 3.0), Point(0.4, 1.1, 0.1), Color(0,1,1));

    debugShapes2.drawBox(Point(0.5, 2.2, 3.0), Point(1.4, 1.1, 0.1), Color(0.5,1,0));

    GLuint resultTexture = firstPassBuffer[0];

    /* Loop until the user closes the window */
    while (!window->shouldClose())
    {

        //START OF SCENE OBJECTS RENDERING
        firstPassBuffer.bind();

        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClearColor(0,0,0, 1);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_FRONT);

        //The PBR shader
        shader.use();
        shader.setFloat("threshold", settingsLayer.getTreshold());

        //PASS LIGHTS TO SHADER         //LIGHTS ARE UPDATED EVERY FRAME, MAKE IT ONLY WHEN THEY ARE MOVED
        for(unsigned i = 0; i < lights.size(); i++)
        {
            lights[i]->sendToShader(shader, i);
        }

        mainScene.drawObjects(); //draws object

        glBindVertexArray(0); //unbind the last vertex array object which belongs to the last rendered mesh
                              //sinse debug doesnt use VAO and doesnt bind one

        //IF WE DONT STOP THE DRAWING TO THE ATTACHMENT1 THE OTHER SHADERS WILL DRAW TO THE BLOOM TEXTURE TOO
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        //END OF SCENE OBJECTS RENDERING - PUT THAT IN THE FUNCTION
                              
        //debugShapes.drawDebugShapes(mainScene.getActiveCamera());
        debugShapes2.drawDebugShapes(mainScene.getActiveCamera());

        lights[0]->drawDebug();
        lights[1]->drawDebug();
        lights[2]->drawDebug();
        lights[3]->drawDebug();

        //mainScene.sceneObjects[6]->drawDebug();

        //make skybox member of scene
        skybox.render(mainScene.getActiveCamera());

        //apply bloom effect, currently the bloom is performance heavy, search for another approach
        if(settingsLayer.isUsingBloom())
        {
            resultTexture = bloomPP.applyEffect(firstPassBuffer, mainUI.getSceneLayer().getFrameBuffer());
            bloomPP.setSteps(settingsLayer.getSteps());
        }
        else
        {
            resultTexture = firstPassBuffer[0];
        }

        //draw the final result to the screne frame buufer, TODO: change the way of gettting gamma and exposure
        FrameQuad::drawFrameQuad(resultTexture, mainUI.getSceneLayer().getFrameBuffer(), settingsLayer.getGamma(), settingsLayer.getExposure());

        //glClearColor(0.15, 0.15, 0.15, 1);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        mainUI.renderUI();

        /* Swap front and back buffers */
        glfwSwapBuffers(window->getGLWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    fileManager.stop();

    return 0;
}