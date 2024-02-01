#include "UI_Scene.h"

UI_Scene* UI_Scene::clone()
{
	return new UI_Scene(*this);
}

UI_Scene::~UI_Scene()
{

}

UI_Scene::UI_Scene(GLFWwindow* window) : window(window)
{
	layerName = "Scene";
}

void UI_Scene::renderLayer()
{
	int width, height, posX, posY;
	glfwGetWindowSize(window, &width, &height);
	glfwGetWindowPos(window, &posX, &posY);

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::Begin(layerName.c_str(), nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();
	glViewport(windowPos.x - posX, height - windowPos.y + posY - windowSize.y, windowSize.x, windowSize.y);
	ImGui::End();
}

