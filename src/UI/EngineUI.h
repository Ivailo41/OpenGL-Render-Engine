#pragma once
#include "Layers/UILayer.h"
#include "Layers/UISceneTree.h"
#include "Layers/UI_Scene.h"
#include "Layers/UI_Settings.h"
#include "Layers/UI_ObjectProperties.h"
#include "Layers/UI_CameraProperties.h"
#include "../Core/Window.h"
#include "../Renderer/Renderer.h"

#include "ImGuizmo.h"

class EngineUI
{
public:
	void addUILayer(UILayer* layer);
	void removeUILayer(unsigned index);
	void removeUILayer(UILayer* layer);
	void clearUILayers();

	void renderUI();

	UI_Scene& getSceneLayer() { return uiSceneLayer; }
	const UI_Settings& getSettingsLayer() const { return uiSettingsLayer; }

public:
	EngineUI(Window* window, FileManager* fileman, Renderer* renderer); //Initing ImGUI here
	EngineUI(const EngineUI& other) = delete;
	EngineUI& operator=(const EngineUI& other) = delete;
	~EngineUI();

private:
	std::string OpenFolderDialog() const;

private:
	std::vector<UILayer*> UIElements;
	static bool isUIOpen;

	FileManager* fileman;

	UI_Scene uiSceneLayer;
	UI_Settings uiSettingsLayer;
	UI_SceneTree uiSceneTree;
	UI_ObjectProperties uiObjectProperties;
	UI_CameraProperties uiCameraProperties;

	Renderer* renderer = nullptr;
};

