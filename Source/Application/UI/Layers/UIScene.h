#pragma once
#include "UILayer.h"
#include "../../Editor/Scene.h"

class UIScene : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UIScene* clone() override;

public:
	UIScene();
	UIScene(const char* name);

private:

};

