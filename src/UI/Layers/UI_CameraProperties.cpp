#include "UI_CameraProperties.h"
//TEMP INCLUDES FOR TESTING
#include "../../Scene/BaseObject.h"

#include "../../Resources/Material.h"

UI_CameraProperties::UI_CameraProperties() : UILayer("Camera Properties")
{
    FOV = 45.0f;
	camSpeed = 1.0f;
}

UI_CameraProperties::UI_CameraProperties(const char* layerName) : UILayer(layerName)
{
    FOV = 45.0f;
    camSpeed = 1.0f;
}

void UI_CameraProperties::renderLayer()
{

    ImGui::Begin(layerName.c_str());

    if (ImGui::SliderFloat("camera FOV", &FOV, 0.0f, 180.0f))
    {
        Scene::activeScene->getActiveCamera()->setFOV(FOV);
    }
    if (ImGui::DragFloat("camera Speed", &camSpeed, 0.1f, 0.1f, 0.0f))
    {
        Scene::activeScene->getActiveCamera()->setSpeed(camSpeed);
    }
    if (ImGui::SliderFloat("Near plane", &near, 0.01f, 100.0f))
    {
        Scene::activeScene->getActiveCamera()->setNear(near);
    }
    if (ImGui::SliderFloat("Far plane", &far, 0.01f, 100.0f))
    {
        Scene::activeScene->getActiveCamera()->setFar(far);
    }

    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    //TEMPORARY DEBUG BUTTON
    static unsigned counter = 0;
    if(ImGui::Button("Add Dummy"))
    {
        std::string name = "Dummy_" + std::to_string(counter);
        BaseObject* dummy = new BaseObject(name); //dynamically allocating new object, remove that code later
		Scene::activeScene->getSelectedObject()->addChild(dummy);
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