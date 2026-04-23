#include "UI_Materials.h"

#include "../IconsFontAwesome7.h"

void UI_Materials::renderLayer() {

    ImGui::Begin(layerName.c_str());

    static float thumbnailSize = 80;
    static float padding = 16;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    float cellSize = thumbnailSize + padding;

    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1) {
        columnCount = 1;
    }

    auto& materials = resourceManager->getMaterials();

    ImGui::Columns(columnCount, "materials", false);

    for(auto& material : materials) {
        ImGui::PushID(material.first.c_str());
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.75f));
        if(ImGui::Button(ICON_FA_FUTBOL, {thumbnailSize, thumbnailSize})) {
            selectedMaterial = &material.second;
        }
        ImGui::PopStyleVar();
        ImGui::PopID();
        ImGui::Text(material.first.c_str());
        ImGui::NextColumn();
    }

    ImGui::Columns(1);

    if (selectedMaterial) {
        ImGui::Text(selectedMaterial->getName().c_str());
        if(ImGui::CollapsingHeader("Diffuse texture")) {
            auto& textures = resourceManager->getTextures();

            for (auto& texture : textures)
            {
                if (ImGui::Selectable(texture.first.c_str()))
                {
                    selectedMaterial->setTexture(texture.second.get(), 0);
                }
            }
        }

        if (ImGui::CollapsingHeader("ORM texture"))
        {
            auto& textures = resourceManager->getTextures();

            for (auto& texture : textures)
            {
                if (ImGui::Selectable(texture.first.c_str()))
                {
                    selectedMaterial->setTexture(texture.second.get(), 1);
                }
            }
        }

        if (ImGui::CollapsingHeader("Normal texture"))
        {
            auto& textures = resourceManager->getTextures();

            for (auto& texture : textures)
            {
                if (ImGui::Selectable(texture.first.c_str()))
                {
                    selectedMaterial->setTexture(texture.second.get(), 2);
                }
            }
        }
    }


    ImGui::End();
}

UILayer *UI_Materials::clone() {
    return new UI_Materials(*this);
}

UI_Materials::UI_Materials(ResourceManager* resourceManager) : UILayer("Materials"), resourceManager(resourceManager) {

}
