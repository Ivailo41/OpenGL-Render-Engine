#pragma once
#include "Window.h"
#include "FileManager.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Scene.h"
#include "../UI/EngineUI.h"

class Engine
{
public:
	bool Init();
	void Run();
	void Shutdown();

	void OnWindowResize(int width, int height);

	static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

public:
	Engine();

private:
	void SetCallbacks();
	void CalculateDeltaTime();

private:
	Window window;
	FileManager fileManager;
	Renderer renderer;
	EngineUI engineUI;

	std::vector<Scene> scenes;

	float deltaTime;
	float lastFrameTime;

	uint16_t windowWidth = 1920;
	uint16_t windowHeight = 1080;
};