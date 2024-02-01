#include "EngineUI.h"
#include "../Includes/imGuiInclude.h"

bool EngineUI::isUIOpen = false;

EngineUI::EngineUI(GLFWwindow* window)
{
    if(isUIOpen)
    {
        return;
    }
    isUIOpen = true;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

EngineUI::~EngineUI()
{
    isUIOpen = false;
	/*ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();*/
}

void EngineUI::addUILayer(UILayer* layer)
{
	UIElements.addObject(*layer);
}

void EngineUI::removeUILayer(unsigned index)
{
	UIElements.removeObject(index);
}

void EngineUI::removeUILayer(UILayer* layer)
{
	UIElements.removeObject(layer);
}

void EngineUI::clearUILayers()
{
	//UIElements.clear();
}

void EngineUI::renderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (size_t i = 0; i < UIElements.getSize(); i++)
    {
        UIElements[i].renderLayer();
    }

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}