#include "UI_Scene.h"

UI_Scene* UI_Scene::clone()
{
	return new UI_Scene(*this);
}

UI_Scene::~UI_Scene()
{

}

UI_Scene::UI_Scene(Window* window) : window(window), fbuffer(2, false), viewMode(false), operation(ImGuizmo::OPERATION::TRANSLATE)
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

    if(ImGui::IsMouseDown(1) && ImGui::IsWindowHovered() && !viewMode)
    {
        viewMode = true;
        glfwSetInputMode(window->getGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if(ImGui::IsMouseReleased(1) && viewMode)
    {
        viewMode = false;
        glfwSetInputMode(window->getGLWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(viewMode)
    {
        Camera::CursorData data(windowSpace.x, windowSpace.y, 0, 0, 0, 0);
        activeCamera->cameraController(window->getGLWindow(), windowSpace.x, windowSpace.y);
    }

    ImGui::Image(
        (ImTextureID)fbuffer[0],
        windowSpace,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );


    //Gizmos
    BaseObject* selectedObject = Scene::activeScene->getSelectedObject();

    //move the imput logic to an imput class
    if(glfwGetKey(window->getGLWindow(), GLFW_KEY_W))
    {
        operation = ImGuizmo::OPERATION::TRANSLATE;
    }
    if (glfwGetKey(window->getGLWindow(), GLFW_KEY_E))
    {
        operation = ImGuizmo::OPERATION::ROTATE;
    }
    if (glfwGetKey(window->getGLWindow(), GLFW_KEY_R))
    {
        operation = ImGuizmo::OPERATION::SCALE;
    }

    if(selectedObject)
    {

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();

        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        //camera
        const Camera* camera = Scene::activeScene->getActiveCamera();
        glm::mat4 transformMat = selectedObject->getGlobalModelMat();

        ImGuizmo::Manipulate(glm::value_ptr(camera->getViewMat()), glm::value_ptr(camera->getPerspectiveMat()),
            operation, ImGuizmo::LOCAL, glm::value_ptr(transformMat));

        if(ImGuizmo::IsUsing())
        {
            //convert global coordinates to local, because the transforms work in local space
            glm::mat4 globalToLocalMatrix = selectedObject->globalToLocalMat(transformMat);
            selectedObject->setTransform(globalToLocalMatrix);
            //glm::vec3 position, rotation, scale;
            //Engine::Math::DecomposeMatrix(globalToLocalMatrix, position, rotation, scale);
            //selectedObject->setPosition(glm::vec3(globalToLocalMatrix[3]));
            //glm::vec3 deltaRotation = glm::degrees(rotation) - selectedObject->getRotation();

            /*selectedObject->setPosition(position);
            selectedObject->rotate(deltaRotation);
            selectedObject->setScale(scale);*/
        }
    }

	ImGui::End();
}

