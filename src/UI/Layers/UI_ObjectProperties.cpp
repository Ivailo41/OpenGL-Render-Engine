#include "UI_ObjectProperties.h"

UI_ObjectProperties::UI_ObjectProperties() : UILayer("Object Properties")
{
    moveSpeed = 0.1f;
}

UI_ObjectProperties::UI_ObjectProperties(const char* layerName) : UILayer(layerName)
{
    moveSpeed = 0.1f;
}

void UI_ObjectProperties::renderLayer()
{

    ImGui::Begin(layerName.c_str());

    if(Scene::activeScene->getSelectedObject() != nullptr)
    {
        transform = Scene::activeScene->getSelectedObject()->getTransform();

        ImGui::Text("selected mesh: %s", Scene::activeScene->getSelectedObject()->getName().c_str());

        ImGui::SliderFloat("move Speed", &moveSpeed, 0.01f, 2.0f);

        if (ImGui::DragFloat3("Move", &transform.position.x, moveSpeed))
        {
            Scene::activeScene->getSelectedObject()->setPosition(transform.position);
        }
        if (ImGui::DragFloat3("Rotate", &transform.rotation.x, moveSpeed))
        {
            Scene::activeScene->getSelectedObject()->setRotation(transform.rotation);
        }
        if (ImGui::DragFloat3("Scale", &transform.scale.x, moveSpeed))
        {
            Scene::activeScene->getSelectedObject()->setScale(transform.scale);
        }
    }
    ImGui::End();
}

UI_ObjectProperties* UI_ObjectProperties::clone()
{
	return new UI_ObjectProperties(*this);
}