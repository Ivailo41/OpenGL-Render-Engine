#include "UI_CameraProperties.h"
//TEMP INCLUDES FOR TESTING
#include "../../../Object.h"

UI_CameraProperties::UI_CameraProperties(Camera* currentCamera) : UILayer("Camera Properties")
{
    FOV = 45.0f;
	camSpeed = 1.0f;
	this->currentCamera = currentCamera;
}

UI_CameraProperties::UI_CameraProperties(const char* layerName, Camera* currentCamera) : UILayer(layerName)
{
    FOV = 45.0f;
    camSpeed = 1.0f;
    this->currentCamera = currentCamera;
}

void UI_CameraProperties::renderLayer()
{

    ImGui::Begin(layerName.c_str());

    if (ImGui::SliderFloat("camera FOV", &FOV, 0.0f, 180.0f))
    {
        currentCamera->setFOV(FOV);
        currentCamera->updateCamera();
    }
    if (ImGui::SliderFloat("camera Speed", &camSpeed, 0.01f, 2.0f))
    {
        currentCamera->setSpeed(camSpeed);
    }
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    //TEMPORARY DEBUG BUTTON
    static unsigned counter = 0;
    if(ImGui::Button("Add Dummy"))
    {
        Object dummyObject;
		BaseObject* dummy = &dummyObject;
        std::string name = "Dummy_" + std::to_string(counter);
		dummy->setName(name);
		Scene::activeScene->getSelectedObject()->addChild(dummyObject);
        counter++;
    }
    //Implement later
    /*if (ImGui::Combo("Combo", &cameraNumber, "MainCamera\0OtherCamera\0"))
    {
        switch (cameraNumber)
        {
        case 0:
            currentCamera = &mainCamera;
            break;
        case 1:
            currentCamera = &otherCamera;
            break;
        default:
            break;
        }
        currentCamera->updateCamera();
        FOV = currentCamera->getFOV();
    }
     ImGui::Checkbox("Use camera as light", &cameraLight);
    */

    ImGui::End();
}

UI_CameraProperties* UI_CameraProperties::clone()
{
    return new UI_CameraProperties(*this);
}