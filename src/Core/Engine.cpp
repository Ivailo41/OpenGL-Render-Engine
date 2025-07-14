#include "Engine.h"

bool Engine::Init()
{
    if (!window.init("Render Engine", windowWidth, windowHeight))
    {
        std::cout << "Couldn't initialize window!" << std::endl;
        return false;
    }
    glfwSetWindowUserPointer(window.getGLWindow(), this);

    if (!fileManager.init())
    {
        std::cout << "Couldn't initialize File Manager!" << std::endl;
        return 1;
    }

    fileManager.createDirectory("../assets");

    fileManager.loadShader("PBRShader", "../assets/Shaders/Main/vertexShader.glsl", "../assets/Shaders/Main/fragShader.glsl");
    fileManager.loadShader("BloomShader", "../assets/Shaders/PostProcess/Bloom/bloomVertex.glsl", "../assets/Shaders/PostProcess/Bloom/bloomFrag.glsl");
    fileManager.loadShader("BlurShader", "../assets/Shaders/PostProcess/Blur/blurVertex.glsl", "../assets/Shaders/PostProcess/Blur/blurFrag.glsl");
    fileManager.loadShader("DebugShader", "../assets/Shaders/Debug/debugVertex.glsl", "../assets/Shaders/Debug/debugFrag.glsl");
    fileManager.loadShader("FramequadShader", "../assets/Shaders/PostProcess/FrameQuad/FrameQuadVertex.glsl", "../assets/Shaders/PostProcess/FrameQuad/FrameQuadFrag.glsl");
    fileManager.loadShader("SkyboxShader", "../assets/Shaders/Skybox/SkyboxVertex.glsl", "../assets/Shaders/Skybox/SkyboxFrag.glsl");
    fileManager.loadShader("ShadowShader", "../assets/Shaders/Shadow/shadowVertex.glsl", "../assets/Shaders/Shadow/shadowFrag.glsl", "../assets/Shaders/Shadow/shadowGeometry.glsl");
    fileManager.loadShader("TangentShader", "../assets/Shaders/Debug/tangentVertex.glsl", "../assets/Shaders/Debug/tangentFrag.glsl", "../assets/Shaders/Debug/tangentGeometry.glsl");

    if (!renderer.init(&window))
    {
        std::cout << "Couldn't initialize Renderer!" << std::endl;
        return 1;
    }

    //Initing ImGUI here
    if (!engineUI.init())
    {
        std::cout << "Couldn't initialize UI!" << std::endl;
        return 1;
    }

    SetCallbacks();

    //SCENE CREATION ABSTRACT THIS PART LATER INTO SCENE MANAGER OR SCENE LOADER
    scenes.push_back(Scene());
    Scene* mainScene = &scenes[0];
    Scene::activeScene = mainScene;

    Camera* mainCamera = new Camera;
    //mainScene.sceneObjects.push_back(mainCamera_p);
    mainScene->addObject(mainCamera);
    mainScene->setActiveCamera(mainCamera);

    Camera* otherCamera = new Camera;
    //mainScene.sceneObjects.push_back(otherCamera_p);
    mainScene->addObject(otherCamera);

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
    Skybox* skybox = new Skybox(cubemapPaths);
    mainScene->activeSkybox = skybox;

    mainCamera->setFOV(90.0f);
    mainCamera->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    otherCamera->setFOV(30.0f);

    //Hard coded lights
    for (size_t i = 0; i < 4; i++)
    {
        mainScene->addObject(new PointLight(std::string("PointLight_" + std::to_string(i))));
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
                                            "../assets/AK203/Set4_Normal.png" };

        fileManager.loadTextures(texturePaths);

        // dynamically loaded object
        if (fileManager.loadOBJ("../assets/AK203/AK203.obj"))
        {
            mainScene->materials[2]->setTexture(mainScene->textures[0], 0);
            mainScene->materials[2]->setTexture(mainScene->textures[1], 1);
            mainScene->materials[2]->setTexture(mainScene->textures[2], 2);

            mainScene->materials[0]->setTexture(mainScene->textures[3], 0);
            mainScene->materials[0]->setTexture(mainScene->textures[4], 1);
            mainScene->materials[0]->setTexture(mainScene->textures[5], 2);

            mainScene->materials[3]->setTexture(mainScene->textures[6], 0);
            mainScene->materials[3]->setTexture(mainScene->textures[7], 1);
            mainScene->materials[3]->setTexture(mainScene->textures[8], 2);

            mainScene->materials[1]->setTexture(mainScene->textures[9], 0);
            mainScene->materials[1]->setTexture(mainScene->textures[10], 1);
            mainScene->materials[1]->setTexture(mainScene->textures[11], 2);
        }
    }

    //that will fix the snap after entering camera controll, would need to set some values in the constructors to notuse this line
    mainScene->getActiveCamera()->rotateCam(glm::vec3(0, 0, 0));

    /*Bloom bloomPP(blurShader, bloomShader);
    bloomPP.setSteps(20);*/

    //this should be in the scene class
    renderer.debugShapes.drawBox(Point(0.5, 2.2, 3.0), Point(1.4, 1.1, 0.1), Color(0.5, 1, 0));
}

void Engine::Run()
{
    Scene* activeScene = Scene::activeScene;

    while (!window.shouldClose())
    {
        CalculateDeltaTime();

        activeScene->updateObjects(deltaTime);

        renderer.renderScene(activeScene, &window); //render the scene with the renderer

        //This can later go into input handling class
        if (engineUI.getSceneLayer().isViewMode())
        {
            ImVec2 windowSpace = engineUI.getSceneLayer().getWindowSpace();

            activeScene->getActiveCamera()->setAspectRatio(windowSpace.x, windowSpace.y);
            activeScene->getActiveCamera()->updateCamera();

            Camera::CursorData data(windowSpace.x, windowSpace.y, 0, 0, 0, 0);
            activeScene->getActiveCamera()->cameraController(window.getGLWindow(), windowSpace.x, windowSpace.y, deltaTime);
        }

        engineUI.renderUI();
        /* Swap front and back buffers */
        glfwSwapBuffers(window.getGLWindow());

        /* Poll for and process events */
        window.pollEvents();
    }
}

void Engine::Shutdown()
{
    renderer.stop();
    fileManager.stop();
    window.stop();
}

void Engine::OnWindowResize(int width, int height)
{
    renderer.onWindowResize(width, height);
    window.setWidth(width); //reset the viewport to the new window size
    window.setHeight(height);
}

void Engine::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if(engine)
    {
        engine->OnWindowResize(width, height);
    }
}

Engine::Engine() : engineUI(&window, &fileManager, &renderer)
{

}

void Engine::SetCallbacks()
{
    glfwSetFramebufferSizeCallback(window.getGLWindow(), FrameBufferResizeCallback);
}

void Engine::CalculateDeltaTime()
{
    double currentFrameTime = glfwGetTime();
    deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
    lastFrameTime = currentFrameTime;
}
