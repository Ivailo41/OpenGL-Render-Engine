#include "UI_Scene.h"

UI_Scene* UI_Scene::clone()
{
	return new UI_Scene(*this);
}

UI_Scene::~UI_Scene()
{

}

UI_Scene::UI_Scene(GLFWwindow* window, unsigned texture) : window(window), texture(texture)
{
	layerName = "Scene";
}

void UI_Scene::renderLayer()
{
	ImGui::Begin(layerName.c_str());

    windowSpace = ImGui::GetContentRegionAvail();
    if(ImGui::IsMouseDown(1) && ImGui::IsWindowHovered())
    {
        Scene::activeScene->getActiveCamera()->cameraController(window, windowSpace.x, windowSpace.y);
    }

    ImGui::Image(
        (ImTextureID)texture,
        windowSpace,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

	ImGui::End();
}

