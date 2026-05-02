#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <cassert>
#include "Logger.h"

class Window
{
public:
	bool init(const std::string& name, unsigned width, unsigned heigth);
	void stop();

	GLFWwindow* getGLWindow() const { return window; }
	unsigned getWidth() const;
	unsigned getHeight() const;
	bool isRunning() const { return running; }

	//temporary using these setters until the resize callback is moved inside the window class
	void setWidth(unsigned width) { this->width = width; }
	void setHeight(unsigned height) { this->height = height; }

	void onWindowResize(int width, int height);

	void setSize(unsigned width, unsigned height);

	bool shouldClose() const;
	void resetViewport() const
	{
		glViewport(0, 0, width, height);
	}

	void pollEvents() const;

	//GLFWwindow* operator()() { return window; }

public:
	Window();
	Window(const Window& other) = delete;

private:
	bool running;
	unsigned width, height;
	std::string name;
	GLFWwindow* window;

private:
	void operator=(const Window& other) = delete;
};