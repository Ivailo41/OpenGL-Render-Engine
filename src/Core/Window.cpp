#include "Window.h"
#include <stb_image.h>

bool Window::init(const std::string& name, unsigned width, unsigned height)
{
    LOG_TRACE("Initializing window!");

    assert(!running); //Make sure init() is called only once

    if (!glfwInit())
    {
		LOG_ERROR("Could not initialise glfw!");
        return false;
    }

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
		LOG_ERROR("Could not initialise window!");
        return false;
    }

    this->name = name;
    this->width = width;
    this->height = height;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, nullptr);

    //DISABLE VSYNC
    glfwSwapInterval(0);

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
        stop();
		LOG_ERROR("Could not initialise GLEW!");
        return false;
    }

    //Test setting window icon
    GLFWimage icon;
    icon.pixels = stbi_load("../assets/Icon/icon.png", &icon.width, &icon.height, nullptr, 0);
    if (icon.pixels)
    {
        glfwSetWindowIcon(window, 1, &icon);
        stbi_image_free(icon.pixels);
    }
    else
    {
		LOG_ERROR("Couldn't load icon!");
    }

    running = true;
    //End of initialization
}

void Window::stop()
{
	LOG_TRACE("Shutting down window!");
    glfwDestroyWindow(window);
    glfwTerminate();
    running = false;
}

//Window* Window::getInstance()
//{
//	return instance;
//}

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

void Window::onWindowResize(int width, int height)
{
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void Window::setSize(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

Window::Window() : running(false), window(nullptr), width(0), height(0), name("Uninitialised Window")
{

}
