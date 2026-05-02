#pragma once
#include "Window.h"
#include "FileManager.h"
#include "Logger.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Scene.h"
#include "../UI/EngineUI.h"

class Engine
{
public:
	bool init();
	void run();
	void shutdown();

	void onWindowResize(int width, int height);

	static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

public:
	Engine();

private:
	void setCallbacks();
	void calculateDeltaTime();

private:
	Window window;
	FileManager fileManager;
	ResourceManager resourceManager;
	Renderer renderer;
	EngineUI engineUI;

	std::vector<Scene> scenes;

	float deltaTime;
	float lastFrameTime;

	uint16_t windowWidth = 1920;
	uint16_t windowHeight = 1080;
};