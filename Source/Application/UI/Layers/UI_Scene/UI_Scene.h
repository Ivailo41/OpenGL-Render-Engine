#pragma once
#include "../UILayer.h"
#include "../../../glm/glm.hpp"

class UI_Scene : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Scene* clone() override;

public:
	UI_Scene(GLFWwindow* window);
	~UI_Scene() override;

private:
	GLFWwindow* window;
};