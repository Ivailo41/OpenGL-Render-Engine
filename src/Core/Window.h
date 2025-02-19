#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <cassert>

class Window
{
public:
	bool init(const std::string& name, unsigned width, unsigned heigth);
	void stop();
	//static Window* getInstance();

	GLFWwindow* getGLWindow() const { return window; }
	unsigned getWidth() const;
	unsigned getHeight() const;

	bool shouldClose() const;

public:
	Window();
	Window(const Window& other) = delete;

private:
	bool isRunning;
	unsigned width, height;
	std::string name;
	GLFWwindow* window;

private:
	void operator=(const Window& other) = delete;
};