#pragma once
#include "UILayer.h"
#include "../../Scene/Scene.h"

class UI_SceneTree : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_SceneTree* clone() override;

public:
	UI_SceneTree();
	UI_SceneTree(const char* name);

private:

};

