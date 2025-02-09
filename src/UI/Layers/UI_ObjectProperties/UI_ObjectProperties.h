#pragma once
#include "../UILayer.h"
#include "../../../glm/glm.hpp"

class UI_ObjectProperties : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_ObjectProperties* clone() override;

public:
	UI_ObjectProperties();
	UI_ObjectProperties(const char*);

private:
	glm::vec3 rotation;
	glm::vec3 position;
	glm::vec3 scale;

	float moveSpeed;
};

