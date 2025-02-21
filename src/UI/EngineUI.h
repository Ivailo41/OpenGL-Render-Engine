#pragma once
#include "Layers/UILayer.h"
#include "Layers/UISceneTree.h"
#include "Layers/UI_Scene.h"
#include "Layers/UI_Settings.h"
#include "Layers/UI_ObjectProperties.h"
#include "Layers/UI_CameraProperties.h"
#include "../Core/Window.h"

#include "ImGuizmo.h"



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
	EngineUI(Window* window, FileManager* fileman); //Initing ImGUI here
	EngineUI(const EngineUI& other) = delete;
	EngineUI& operator=(const EngineUI& other) = delete;
	~EngineUI();

private:
	std::vector<UILayer*> UIElements;
	static bool isUIOpen;
	FileManager* fileman;

	UI_Scene uiSceneLayer;
	UI_Settings uiSettingsLayer;
	UI_SceneTree uiSceneTree;
	UI_ObjectProperties uiObjectProperties;
	UI_CameraProperties uiCameraProperties;
};

