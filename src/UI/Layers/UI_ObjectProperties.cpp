#include "UI_ObjectProperties.h"

UI_ObjectProperties::UI_ObjectProperties() : UILayer("Object Properties")
{
    rotation = glm::vec3(0.0f);
    position = glm::vec3(0.0f);
    scale = glm::vec3(0.0f);

    moveSpeed = 0.1f;
}

UI_ObjectProperties::UI_ObjectProperties(const char* layerName) : UILayer(layerName)
{
    rotation = glm::vec3(0.0f);
    position = glm::vec3(0.0f);
    scale = glm::vec3(0.0f);

    moveSpeed = 0.1f;
}

void UI_ObjectProperties::renderLayer()
{

    ImGui::Begin(layerName.c_str());

    if(Scene::activeScene->getSelectedObject() != nullptr)
    {
        rotation = Scene::activeScene->getSelectedObject()->getRotation();
        position = Scene::activeScene->getSelectedObject()->getPosition();
        scale = Scene::activeScene->getSelectedObject()->getScale();

        ImGui::Text("selected mesh: %s", Scene::activeScene->getSelectedObject()->getName().c_str());

        ImGui::SliderFloat("move Speed", &moveSpeed, 0.01f, 2.0f);

        if (ImGui::DragFloat3("Move", &position.x, moveSpeed))
        {
            Scene::activeScene->getSelectedObject()->setPosition(position);
        }
        if (ImGui::DragFloat3("Rotate", &rotation.x, moveSpeed))
        {
            Scene::activeScene->getSelectedObject()->setRotation(rotation);
        }
        if (ImGui::DragFloat3("Scale", &scale.x, moveSpeed))
        {
            Scene::activeScene->getSelectedObject()->setScale(scale);
        }
    }
    ImGui::End();
}

UI_ObjectProperties* UI_ObjectProperties::clone()
{
	return new UI_ObjectProperties(*this);
}