#pragma once
#include "Layers/UILayer.h"
#include "Layers/UISceneTree.h"
#include "Layers/UI_Scene.h"
#include "Layers/UI_Settings.h"
#include "Layers/UI_ObjectProperties.h"
#include "Layers/UI_CameraProperties.h"
#include "Layers/UI_Console.h"
#include "../Core/Window.h"
#include "../Renderer/Renderer.h"

#include "ImGuizmo.h"

class EngineUI
{
public:

	bool init(); //Initing ImGUI here

	void addUILayer(UILayer* layer);
	void removeUILayer(unsigned index);
	void removeUILayer(UILayer* layer);
	void clearUILayers();

	void renderUI();

	UI_Scene& getSceneLayer() { return uiSceneLayer; }
	const UI_Settings& getSettingsLayer() const { return uiSettingsLayer; }
	UI_Console& getConsoleLayer() { return uiConsole; }

public:
	EngineUI(Window* window, FileManager* fileman, Renderer* renderer);
	EngineUI(const EngineUI& other) = delete;
	EngineUI& operator=(const EngineUI& other) = delete;
	~EngineUI();

private:
	static std::string OpenFolderDialog() ;

private:
	std::vector<UILayer*> UIElements;
	static bool isUIOpen;


	UI_Scene uiSceneLayer;
	UI_Settings uiSettingsLayer;
	UI_SceneTree uiSceneTree;
	UI_ObjectProperties uiObjectProperties;
	UI_CameraProperties uiCameraProperties;
	UI_Console uiConsole;

	Window* window = nullptr;
	FileManager* fileman = nullptr;
	Renderer* renderer = nullptr;
};

