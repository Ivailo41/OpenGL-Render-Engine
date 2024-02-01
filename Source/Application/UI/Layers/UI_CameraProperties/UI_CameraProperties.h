#pragma once
#include "../UILayer.h"
#include "../../../glm/glm.hpp"
#include "../../../Camera.h"

class UI_CameraProperties : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_CameraProperties* clone() override;

public:
	UI_CameraProperties(Camera*);
	UI_CameraProperties(const char*, Camera*);

private:
	float FOV = 50.0f;
	float camSpeed = 0.01f;
	int counter = 0;

	Camera* currentCamera;
};

