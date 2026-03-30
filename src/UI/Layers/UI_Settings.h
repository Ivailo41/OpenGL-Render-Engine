#pragma once
#include "UILayer.h"
#include "../../Resources/Shader.h"
#include "../../Renderer/Renderer.h"

class UI_Settings : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Settings* clone() override;

public:
	UI_Settings(Renderer* renderer);

private:
	Renderer* renderer;
};