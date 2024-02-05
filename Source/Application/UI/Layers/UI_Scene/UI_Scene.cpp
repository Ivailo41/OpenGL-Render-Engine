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

    ImGui::Image(
        (ImTextureID)texture,
        ImGui::GetContentRegionAvail(),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

	ImGui::End();
}

