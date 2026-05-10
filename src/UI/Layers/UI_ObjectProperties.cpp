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

    if(Scene::activeScene != nullptr && Scene::activeScene->getSelectedObject() != nullptr)
    {
        ImGui::Text("selected mesh: %s", Scene::activeScene->getSelectedObject()->getName().c_str());

		TransformComponent* transformComp = Scene::activeScene->getSelectedObject()->getComponent<TransformComponent>();
        if(transformComp != nullptr) {
            Transform transform = transformComp->getTransform();

            ImGui::SliderFloat("move Speed", &moveSpeed, 0.01f, 2.0f);

            if (ImGui::DragFloat3("Move", &transform.position.x, moveSpeed))
            {
                transformComp->setPosition(transform.position);
            }
            if (ImGui::DragFloat3("Rotate", &transform.rotation.x, moveSpeed))
            {
                transformComp->setRotation(transform.rotation);
            }
            if (ImGui::DragFloat3("Scale", &transform.scale.x, moveSpeed))
            {
                transformComp->setScale(transform.scale);
            }
		}

		CameraComponent* cameraComp = Scene::activeScene->getSelectedObject()->getComponent<CameraComponent>();
        if (cameraComp != nullptr) {

			float FOV = cameraComp->getFOV();
			float camSpeed = cameraComp->getSpeed();
			float near = cameraComp->getNear();
			float far = cameraComp->getFar();

            if (ImGui::SliderFloat("camera FOV", &FOV, 0.0f, 180.0f))
            {
                cameraComp->setFOV(FOV);
            }
            if (ImGui::DragFloat("camera Speed", &camSpeed, 0.1f, 0.1f, 0.0f))
            {
               cameraComp->setSpeed(camSpeed);
            }
            if (ImGui::SliderFloat("Near plane", &near, 0.01f, 100.0f))
            {
                cameraComp->setNear(near);
            }
            if (ImGui::SliderFloat("Far plane", &far, 0.01f, 100.0f))
            {
                cameraComp->setFar(far);
            }
        }

		LightComponent* lightComp = Scene::activeScene->getSelectedObject()->getComponent<LightComponent>();
        if (lightComp != nullptr) {
            
			float intensity = lightComp->getIntensity();
			float ambient = lightComp->getAmbientStrength();
			glm::vec3 lightColor = lightComp->getLightColor();
			float shadowNear = lightComp->getShadowNear();
			float shadowFar = lightComp->getShadowFar();

            if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 20.0f))
            {
                lightComp->setIntensity(intensity);
            }
            if (ImGui::SliderFloat("Ambient", &ambient, 0.0f, 20.0f))
            {
                lightComp->setAmbientStrength(ambient);
            }
            if(ImGui::ColorEdit3("Light Color", (float*)&lightColor))
            {
                lightComp->setLightColor(lightColor.r, lightColor.g, lightColor.b);
			}
            if (ImGui::SliderFloat("Shadow Near", &shadowNear, 0.0f, 500.0f))
            {
                lightComp->setShadowNear(shadowNear);
            }
            if (ImGui::SliderFloat("Shadow Far", &shadowFar, 0.0f, 500.0f))
            {
                lightComp->setShadowFar(shadowFar);
            }
        }

    }
    ImGui::End();
}

UI_ObjectProperties* UI_ObjectProperties::clone()
{
	return new UI_ObjectProperties(*this);
}