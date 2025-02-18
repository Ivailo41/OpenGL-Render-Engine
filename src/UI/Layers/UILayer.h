#pragma once
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../../Scene/Scene.h"

class UILayer
{
public:
	virtual void renderLayer() = 0;
	virtual UILayer* clone() = 0;

	void setLayerName(const char*);

public:
	UILayer();
	UILayer(const char*);
	virtual ~UILayer();

protected:
	bool isLayerOpen = false;
	std::string layerName;
};

