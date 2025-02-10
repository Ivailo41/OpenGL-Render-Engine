#pragma once
#include "../UILayer.h"
#include <glm.hpp>
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
	float near = 0.1f;
	float far = 100.0f;
	int counter = 0;

	//TEMP
	float roughness = 0.5;
	float metalic = 0.5;

	Camera* currentCamera;
};

