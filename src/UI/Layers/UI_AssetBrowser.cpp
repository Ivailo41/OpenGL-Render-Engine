#include "UI_AssetBrowser.h"

#include "../IconsFontAwesome7.h"

void UI_AssetBrowser::renderLayer() {

    ImGui::Begin(layerName.c_str());

    static float thumbnailSize = 80;
    static float padding = 16;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    float cellSize = thumbnailSize + padding;

    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1) {
        columnCount = 1;
    }

    auto& models = resourceManager->getModels();

    ImGui::Columns(columnCount, "models", false);

    for(auto& model : models) {
        ImGui::PushID(model.first.c_str());
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.75f));
        if(ImGui::Button(ICON_FA_FILE, {thumbnailSize, thumbnailSize})) {
            Scene::activeScene->instanceModel(model.first);
        }
        ImGui::PopStyleVar();
        ImGui::PopID();
        ImGui::Text(model.first.c_str());
        ImGui::NextColumn();
    }

    ImGui::End();
}

UILayer *UI_AssetBrowser::clone() {
    return new UI_AssetBrowser(*this);
}

UI_AssetBrowser::UI_AssetBrowser(ResourceManager* resourceManager) : UILayer("Asset browser"), resourceManager(resourceManager) {

}
