#pragma once
#include "UILayer.h"
#include "../../Resources/Shader.h"
#include "../../Renderer/Renderer.h"

//temp
#include "../../Core/FileManager.h"

class UI_Settings : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Settings* clone() override;

public:
	UI_Settings(FileManager* fileman, Renderer* renderer);

private:
	//temp code for the model importing
	FileManager* fileman;
	Renderer* renderer;
};