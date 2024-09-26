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

    // create a default directory for the resources
    FileManager::createDirectory("resources");

    //CREATE SHADER
    Shader shader("Shaders/Main/vertexShader.glsl", "Shaders/Main/fragShader.glsl");

    //CREATE BLOOM SHADER
    Shader bloomShader("Shaders/PostProcess/Bloom/bloomVertex.glsl", "Shaders/PostProcess/Bloom/bloomFrag.glsl");

    //CREATE BLUR SHADER
    Shader blurShader("Shaders/PostProcess/Blur/blurVertex.glsl", "Shaders/PostProcess/Blur/blurFrag.glsl");

    //CREATE DEBUG SHADER
    Shader debugShader("Shaders/Debug/debugVertex.glsl", "Shaders/Debug/debugFrag.glsl");

    //CREATE FRAMEQUAD SHADER
    Shader frameQuadShader("Shaders/PostProcess/FrameQuad/FrameQuadVertex.glsl", "Shaders/PostProcess/FrameQuad/FrameQuadFrag.glsl");

    //CREATE SKYBOX SHADER
    Shader skyboxShader("Shaders/Skybox/SkyboxVertex.glsl", "Shaders/Skybox/SkyboxFrag.glsl");

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

    std::string cubemapPaths[6] = 
    { 
        "resources/Skybox/right.jpg",
        "resources/Skybox/left.jpg",
        "resources/Skybox/top.jpg",
        "resources/Skybox/bottom.jpg",
        "resources/Skybox/front.jpg",
        "resources/Skybox/back.jpg" 
    };

    Cubemap testCubeMap(FileManager::loadCubemap(cubemapPaths));
    Skybox skybox(&testCubeMap, &skyboxShader);

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
    lights[0]->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    lights[0]->setIntensity(100);

    lights[1]->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    lights[1]->setIntensity(100);

    lights[2]->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    lights[2]->setIntensity(100);

    lights[3]->setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    lights[3]->setIntensity(100);

    lights[0]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));
    lights[1]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));
    lights[2]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));
    lights[3]->debugLinesContainer.pushLine(Line(Point(0, 0, 0), Point(0.1, 0, 0)));

    //Loading textures and setting materials untill I make it through the UI
    {
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

        /*Texture set1Base(FileManager::loadTexture("resources/AK74M/1_BaseColor.png"));
        Texture set1ORM(FileManager::loadTexture("resources/AK74M/1_OcclusionRoughnessMetallic.png"));
        Texture set1Normal(FileManager::loadTexture("resources/AK74M/1_Normal.png"));

        Texture set2Base(FileManager::loadTexture("resources/AK74M/2_BaseColor.png"));
        Texture set2ORM(FileManager::loadTexture("resources/AK74M/2_OcclusionRoughnessMetallic.png"));
        Texture set2Normal(FileManager::loadTexture("resources/AK74M/2_Normal.png"));

        Texture set3Base(FileManager::loadTexture("resources/AK74M/3_BaseColor.png"));
        Texture set3ORM(FileManager::loadTexture("resources/AK74M/3_OcclusionRoughnessMetallic.png"));
        Texture set3Normal(FileManager::loadTexture("resources/AK74M/3_Normal.png"));*/

        /*Texture::loadTexture("resources/Brick_Base.jpg");
        Texture::loadTexture("resources/Brick_Normal.jpg");
        Texture::loadTexture("resources/Marble_ORM5.jpg");*/
        

        /*Texture::loadTexture("resources/Marble_Albedo.jpg");
        Texture::loadTexture("resources/Marble_Normal.jpg");
        Texture::loadTexture("resources/Marble_ORM.png");*/

        // dynamically load object
        if (!mainScene.loadObject("resources/AK74M/AK74M.obj"))
        {
            std::cout << "Could not load object" << std::endl;;
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
        Material::getMaterial(1)->setTexture(Texture::textures[7],2);*/

        /*try
        {
            Material::getMaterial(0)->setTexture(set3Base, 0);
            Material::getMaterial(0)->setTexture(set3ORM, 1);
            Material::getMaterial(0)->setTexture(set3Normal, 2);

            Material::getMaterial(2)->setTexture(set2Base, 0);
            Material::getMaterial(2)->setTexture(set2ORM, 1);
            Material::getMaterial(2)->setTexture(set2Normal, 2);

            Material::getMaterial(1)->setTexture(set1Base, 0);
            Material::getMaterial(1)->setTexture(set1ORM, 1);
            Material::getMaterial(1)->setTexture(set1Normal, 2);
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }*/

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

    EngineUI mainUI(window);

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
        firstPassBuffer.bind();

        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0, 1);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_FRONT);

        skybox.render(mainScene.getActiveCamera());

        shader.use();
        shader.setFloat("threshold", mainUI.getSettingsLayer().getTreshold());

        //PASS LIGHTS TO SHADER         //LIGHTS ARE UPDATED EVERY FRAME, MAKE IT ONLY WHEN THEY ARE MOVED
        for(unsigned i = 0; i < lights.size(); i++)
        {
            lights[i]->sendToShader(shader, i);
        }

        mainScene.drawObjects(); //draws object

        glBindVertexArray(0); //unbind the last vertex array object which belongs to the last rendered mesh
                              //sinse debug doesnt use VAO and doesnt bind one
                              
        //debugShapes.drawDebugShapes(mainScene.getActiveCamera());
        debugShapes2.drawDebugShapes(mainScene.getActiveCamera());

        lights[0]->drawDebug();
        lights[1]->drawDebug();
        lights[2]->drawDebug();
        lights[3]->drawDebug();

        //mainScene.sceneObjects[6]->drawDebug();

        //apply bloom effect, currently the bloom is performance heavy, search for another approach
        if(mainUI.getSettingsLayer().getBloom())
        {
            resultTexture = bloomPP.applyEffect(firstPassBuffer, mainUI.getSceneLayer().getFrameBuffer());
            bloomPP.setSteps(mainUI.getSettingsLayer().getSteps());
        }
        else
        {
            resultTexture = firstPassBuffer[0];
        }

        //draw the final result to the screne frame buufer
        FrameQuad::drawFrameQuad(resultTexture, mainUI.getSceneLayer().getFrameBuffer());

        //glClearColor(0.15, 0.15, 0.15, 1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        mainUI.renderUI();

        /* Swap front and back buffers */
        glfwSwapBuffers(window->getGLWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}