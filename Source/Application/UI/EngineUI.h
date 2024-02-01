#pragma once
#include "../ObjectArray.h"
#include "Layers/UILayer.h"
#include <GLFW/glfw3.h>

class EngineUI
{
public:
	void addUILayer(UILayer* layer);
	void removeUILayer(unsigned index);
	void removeUILayer(UILayer* layer);
	void clearUILayers();

	void renderUI();

public:
	EngineUI(GLFWwindow* window);
	EngineUI() = delete;
	EngineUI(const EngineUI& other) = delete;
	EngineUI& operator=(const EngineUI& other) = delete;
	~EngineUI();

private:
	ObjectArray<UILayer> UIElements;
	static bool isUIOpen;
};

