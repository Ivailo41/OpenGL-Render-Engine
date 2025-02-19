#include "Window.h"
#include <stb_image.h>

bool Window::init(const std::string& name, unsigned width, unsigned height)
{
    std::cout << "Initializing window!" << std::endl;

    assert(!isRunning); //Make sure init() is called only once

    if (!glfwInit())
    {
        std::cout << "Could not initialise glfw!" << std::endl;
        return false;
    }

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window)
    {
        this->name = name;
        this->width = width;
        this->height = height;

        glfwTerminate();
        std::cout << "Could not initialise window!" << std::endl;
        return false;
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
        stop();
        std::cout << "GLEW ERROR!" << std::endl;
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
        std::cout << "Couldn't load icon!" << std::endl;
    }

    isRunning = true;
    //End of initialization
}

void Window::stop()
{
    std::cout << "Shutting down window!" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    isRunning = false;
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

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

Window::Window() : isRunning(false), window(nullptr), width(0), height(0), name("Uninitialised Window")
{

}
