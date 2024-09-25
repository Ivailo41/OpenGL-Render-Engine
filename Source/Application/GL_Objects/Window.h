#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	static Window* getInstance(const std::string& name, unsigned width, unsigned heigth);
	static Window* getInstance();

	GLFWwindow* getGLWindow() const { return window; }
	unsigned getWidth() const;
	unsigned getHeight() const;

	bool shouldClose() const;

public:
	Window(const Window& other) = delete;
	~Window();

private:
	std::string name;
	unsigned width, height;
	GLFWwindow* window;

	static Window* instance;

private:
	Window(const std::string& name, unsigned width, unsigned heigth);
	void operator=(const Window& other) = delete;
};