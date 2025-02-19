#include "UI_Scene.h"

UI_Scene* UI_Scene::clone()
{
	return new UI_Scene(*this);
}

UI_Scene::~UI_Scene()
{

}

UI_Scene::UI_Scene(Window* window) : window(window), fbuffer(2, false)
{
    //create framebuffer that will project the context from the main window to the UI window

    fbuffer.genFrameBuffer(window->getWidth(), window->getHeight());

	layerName = "Scene";
}

void UI_Scene::renderLayer()
{
	ImGui::Begin(layerName.c_str());

    //Fix for the stretching when resizing the window problem, but the camera updates every frame, no idea if I would want that
    windowSpace = ImGui::GetContentRegionAvail();
    //windowSpace = ImGui::GetWindowSize();;
    Camera* activeCamera = Scene::activeScene->getActiveCamera();
    activeCamera->setAspectRatio(windowSpace.x, windowSpace.y);
    activeCamera->updateCamera();

    if(ImGui::IsMouseDown(1) && ImGui::IsWindowHovered())
    {
        /*if(ImGui::IsKeyPressed(ImGuiKey_W))
        {
            activeCamera->translate(activeCamera->getViewDirection() * activeCamera->getSpeed());
        }
        if (ImGui::IsKeyPressed(ImGuiKey_S))
        {
            activeCamera->translate(-activeCamera->getViewDirection() * activeCamera->getSpeed());
        }
        if (ImGui::IsKeyPressed(ImGuiKey_A))
        {
            activeCamera->translate(glm::cross(activeCamera->getViewDirection(), glm::vec3(0, -1, 0)) * activeCamera->getSpeed());
        }
        if (ImGui::IsKeyPressed(ImGuiKey_D))
        {
            activeCamera->translate(glm::cross(activeCamera->getViewDirection(), glm::vec3(0, 1, 0)) * activeCamera->getSpeed());
        }*/
        /*ImVec2 cursorPos = ImGui::GetMousePos();
        ImVec2 offset = ImGui::GetWindowPos();
        std::cout << "X: " << ImGui::GetWindowSize().x << " Y: " << ImGui::GetWindowSize().y << std::endl;*/
        //Camera::CursorData data(windowSpace.x, windowSpace.y, offset.x, offset.y, cursorPos.x, cursorPos.y);
        Camera::CursorData data(windowSpace.x, windowSpace.y, 0,0, 0, 0);

        activeCamera->cameraController(window->getGLWindow(), windowSpace.x, windowSpace.y);
    }

    ImGui::Image(
        (ImTextureID)fbuffer[0],
        windowSpace,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

	ImGui::End();
}

