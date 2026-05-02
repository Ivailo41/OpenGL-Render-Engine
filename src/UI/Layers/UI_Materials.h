#pragma once
#include "UILayer.h"
#include "../../Resources/ResourceManager.h"

class UI_Materials : public UILayer {

public:
    void renderLayer() override;
    UILayer* clone() override;

    UI_Materials(ResourceManager* resourceManager);
private:
    ResourceManager* resourceManager;
    Material* selectedMaterial = nullptr;
};