#pragma once
#include "UILayer.h"
#include <glm.hpp>

class UI_ObjectProperties : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_ObjectProperties* clone() override;

public:
	UI_ObjectProperties();
	UI_ObjectProperties(const char*);

private:
	Transform transform;

	float moveSpeed;
};

