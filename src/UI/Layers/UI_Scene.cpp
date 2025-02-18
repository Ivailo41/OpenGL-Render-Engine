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
    Scene::activeScene->getActiveCamera()->setAspectRatio(windowSpace.x, windowSpace.y);
    Scene::activeScene->getActiveCamera()->updateCamera();

    if(ImGui::IsMouseDown(1) && ImGui::IsWindowHovered())
    {
        Scene::activeScene->getActiveCamera()->cameraController(window->getGLWindow(), windowSpace.x, windowSpace.y);
    }

    ImGui::Image(
        (ImTextureID)fbuffer[0],
        windowSpace,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

	ImGui::End();
}

