#pragma once
#include "UILayer.h"
#include "../../Resources/ResourceManager.h"

class UI_AssetBrowser : public UILayer {

public:
    void renderLayer() override;
    UILayer* clone() override;

    UI_AssetBrowser(ResourceManager* resourceManager);
private:
    ResourceManager* resourceManager;
};