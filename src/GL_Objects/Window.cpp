#include "Window.h"

Window* Window::instance = nullptr;

Window* Window::getInstance(const std::string& name, unsigned width, unsigned heigth)
{
	if(instance == nullptr)
	{
        instance = new Window(name, width, heigth);
	}

	return instance;
}

Window* Window::getInstance()
{
	return instance;
}

unsigned Window::getWidth() const
{
    int width;
    glfwGetWindowSize(window, &width, nullptr);
    return width;
}

unsigned Window::getHeight() const
{
    int height;
    glfwGetWindowSize(window, nullptr, &height);
    return height;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

Window::Window(const std::string& name, unsigned width, unsigned height) : width(width), height(height)
{
    if (!glfwInit())
    {
        throw "Could not initialise glfw!";
    }

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialise window!";
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, nullptr);

    // Getting the primary monitor and setting the window to be fullscreen
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int monitorX, monitorY;
        glfwGetMonitorPos(monitor, &monitorX, &monitorY);
        int monitorWidth = mode->width;
        int monitorHeight = mode->height;

        glfwMaximizeWindow(window);
        //glfwSetWindowPos(window, monitorX, monitorY);
        glfwSetWindowSize(window, monitorWidth, monitorHeight);
        glViewport(0, 0, monitorWidth, monitorHeight);
    }

    if (glewInit() != GLEW_OK)
    {
        throw "GLEW ERROR!";
    }
    //End of initialization
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
	delete instance;
}
