#pragma once
#include "UILayer.h"
#include "../../Renderer/Shader.h"
#include "../../Renderer/Renderer.h"

//temp
#include "../../FileManager.h"

class UI_Settings : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Settings* clone() override;

	//temp
	std::string OpenFolderDialog() const;

public:
	UI_Settings(FileManager* fileman, Renderer* renderer);

private:
	//temp code for the model importing
	FileManager* fileman;
	Renderer* renderer;
};