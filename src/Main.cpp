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

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
#include <memory>

int main(int argc, char* argv[])
{
    //GLFWwindow* window;
    Window* window;
    try
    {
        window = Window::getInstance("Render Engine", 1920, 1080);
    }
    catch(const char* err)
    {
        std::cerr << err << std::endl;
        return 1; //Move that to engine class and stop it if the window throws
    }

    FileManager fileManager;
    fileManager.init();

    // create a default directory for the resources
    fileManager.createDirectory("../assets");

    //Move the shader creation to the startup fo an Engine class
    //CREATE PBR SHADER
    //Shader shader(FileManager::loadShader("Shaders/Main/vertexShader.glsl"), FileManager::loadShader("Shaders/Main/fragShader.glsl"));
    fileManager.loadShader("PBRShader", "../assets/Shaders/Main/vertexShader.glsl", "../assets/Shaders/Main/fragShader.glsl");
    Shader& shader = Shader::shadersList.find("PBRShader").operator*().second;

    //CREATE BLOOM SHADER
    Shader bloomShader(fileManager.loadShader("../assets/Shaders/PostProcess/Bloom/bloomVertex.glsl"), fileManager.loadShader("../assets/Shaders/PostProcess/Bloom/bloomFrag.glsl"));

    //CREATE BLUR SHADER
    Shader blurShader(fileManager.loadShader("../assets/Shaders/PostProcess/Blur/blurVertex.glsl"), fileManager.loadShader("../assets/Shaders/PostProcess/Blur/blurFrag.glsl"));

    //CREATE DEBUG SHADER
    Shader debugShader(fileManager.loadShader("../assets/Shaders/Debug/debugVertex.glsl"), fileManager.loadShader("../assets/Shaders/Debug/debugFrag.glsl"));

    //CREATE FRAMEQUAD SHADER
    Shader frameQuadShader(fileManager.loadShader("../assets/Shaders/PostProcess/FrameQuad/FrameQuadVertex.glsl"), fileManager.loadShader("../assets/Shaders/PostProcess/FrameQuad/FrameQuadFrag.glsl"));

    //CREATE SKYBOX SHADER
    Shader skyboxShader(fileManager.loadShader("../assets/Shaders/Skybox/SkyboxVertex.glsl"), fileManager.loadShader("../assets/Shaders/Skybox/SkyboxFrag.glsl"));

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
        "../assets/Skybox/right.jpg",
        "../assets/Skybox/left.jpg",
        "../assets/Skybox/top.jpg",
        "../assets/Skybox/bottom.jpg",
        "../assets/Skybox/front.jpg",
        "../assets/Skybox/back.jpg" 
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
                                            "../assets/AK203/Set4_Normal.png" };

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
    EngineUI mainUI(window, &fileManager);
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

    // Loop until the user closes the window, put it inside application/engine class
    while (!window->shouldClose())
    {
        //this scope should go inside a renderer class
        {
            //START OF SCENE OBJECTS RENDERING
            firstPassBuffer.bind();

            unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            glDrawBuffers(2, attachments);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            mainUI.renderUI();

            /* Swap front and back buffers */
            glfwSwapBuffers(window->getGLWindow());
        }


        /* Poll for and process events */
        glfwPollEvents();
    }

    fileManager.stop();

    return 0;
}