#pragma once
#include "UILayer.h"
#include "../../Editor/Scene.h"

class UISceneTree : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UISceneTree* clone() override;

public:
	UISceneTree();
	UISceneTree(const char* name);

private:

};

