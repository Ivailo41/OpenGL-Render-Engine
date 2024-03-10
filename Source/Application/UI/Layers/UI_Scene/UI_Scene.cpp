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
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

    unsigned int rbo;

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

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

