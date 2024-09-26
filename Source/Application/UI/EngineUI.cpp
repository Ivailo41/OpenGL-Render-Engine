#include "EngineUI.h"
#include "../Includes/imGuiInclude.h"

bool EngineUI::isUIOpen = false;

EngineUI::EngineUI(Window* window) : uiSceneLayer(window)
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

    ImGui_ImplGlfw_InitForOpenGL(window->getGLWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    //Create default UI layers
    UISceneTree uiSceneTree;
    UI_ObjectProperties uiObjectProperties;
    UI_CameraProperties uiCameraProperties(Scene::activeScene->getActiveCamera());
    addUILayer(&uiSceneTree);
    addUILayer(&uiObjectProperties);
    addUILayer(&uiCameraProperties);
    addUILayer(&uiSceneLayer);
    //addUILayer(&uiSettingsLayer);
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

    //Main layer that contains all the UI elements
    ImGui::NewFrame();

    static bool use_work_area = true;
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
    // Based on your use case you may want one or the other.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (ImGui::Begin("Example: Fullscreen window", nullptr, flags))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Examples"))
            {

                ImGui::EndMenu();
            }
            //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
            if (ImGui::BeginMenu("Tools"))
            {

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    ImGui::End();

    for (size_t i = 0; i < UIElements.getSize(); i++)
    {
        UIElements[i].renderLayer();
    }
    uiSettingsLayer.renderLayer();

    //ImGui::ShowStyleEditor();
    //ImGui::ShowDemoWindow();

    //ImGuiStyle& style = ImGui::GetStyle();
    //style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2, 0.2, 0.2, 1);

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