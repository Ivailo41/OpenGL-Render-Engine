#pragma once
#include "../ObjectArray.h"
#include "Layers/UILayer.h"
#include "Layers/UISceneTree.h"
#include "Layers/UI_Scene/UI_Scene.h"
#include "Layers/UI_Settings/UI_Settings.h"
#include "Layers/UI_ObjectProperties/UI_ObjectProperties.h"
#include "Layers/UI_CameraProperties/UI_CameraProperties.h"
#include "../GL_Objects/Window.h"

class EngineUI
{
public:
	void addUILayer(UILayer* layer);
	void removeUILayer(unsigned index);
	void removeUILayer(UILayer* layer);
	void clearUILayers();

	void renderUI();

	const UI_Scene& getSceneLayer() const { return uiSceneLayer; }
	const UI_Settings& getSettingsLayer() const { return uiSettingsLayer; }

public:
	EngineUI(Window* window); //Initing ImGUI here
	//EngineUI() = delete;
	EngineUI(const EngineUI& other) = delete;
	EngineUI& operator=(const EngineUI& other) = delete;
	~EngineUI();

private:
	ObjectArray<UILayer> UIElements;
	static bool isUIOpen;

	UI_Scene uiSceneLayer;
	UI_Settings uiSettingsLayer;
};

