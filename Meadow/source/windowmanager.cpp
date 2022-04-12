#include "windowmanager.h"
#include "service_locator/locator.h"
//#include <fmt/core.h>

namespace WindowConf {
    const float DEFAULT_SCR_WIDTH = 1920.0f;
    const float DEFAULT_SCR_HEIGHT = 1080.0f;
}
float WindowManager::width = WindowConf::DEFAULT_SCR_WIDTH;
float WindowManager::height = WindowConf::DEFAULT_SCR_HEIGHT;
WindowManager::WindowManager(): m_window(nullptr)
{
}

WindowManager::~WindowManager()
{
    glfwTerminate();
    glfwDestroyWindow(m_window);
}

bool WindowManager::createWindow(std::string title)
{
    // glfw: initialize and configure
// ------------------------------
    if (!glfwInit()) {
        Locator::getLogger()->getLogger()->info("Failed to initialize glfw\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
   // --------------------
    m_window = glfwCreateWindow(WindowConf::DEFAULT_SCR_WIDTH, WindowConf::DEFAULT_SCR_HEIGHT, title.c_str(), NULL, NULL);
    if (m_window == NULL)
    {
        Locator::getLogger()->getLogger()->info("Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /*glfwSetCursorPosCallback(window, &mouseCallback);
    glfwSetScrollCallback(window, &scrollCallback);*/
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    return true;
}

bool WindowManager::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void WindowManager::swapBuffers()
{
    glfwSwapBuffers(m_window);
}


WindowManager::GLProc WindowManager::getProcAddress()
{
    return (GLProc)glfwGetProcAddress;
}

GLFWwindow* WindowManager::getWindow()
{
    return m_window;
}

float WindowManager::getTime()
{
    return (float)glfwGetTime();
}

void WindowManager::pollEvents()
{
    glfwPollEvents();
}

void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    Locator::getRenderer()->setViewportSize(width, height);
}
