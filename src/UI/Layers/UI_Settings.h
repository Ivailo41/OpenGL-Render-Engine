#pragma once
#include "UILayer.h"
#include "../../Renderer/Renderer.h"
#include "../../Resources/ResourceManager.h"

class UI_Settings : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Settings* clone() override;

public:
	UI_Settings(Renderer* renderer, ResourceManager* resourceManager);

private:
	Renderer* renderer;
	ResourceManager* resourceManager;
};