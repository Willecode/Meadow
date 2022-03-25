#include "windowmanager.h"
#include <fmt/printf.h>

namespace WindowConf {
    const float DEFAULT_SCR_WIDTH = 1920.0f;
    const float DEFAULT_SCR_HEIGHT = 1080.0f;
}
float WindowManager::width = WindowConf::DEFAULT_SCR_WIDTH;
float WindowManager::height = WindowConf::DEFAULT_SCR_HEIGHT;
WindowManager::WindowManager(): m_window(nullptr)
{
}

bool WindowManager::createWindow()
{
    // glfw: initialize and configure
// ------------------------------
    if (!glfwInit()) {
        fmt::print("Failed to initialize glfw\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
   // --------------------
    m_window = glfwCreateWindow(WindowConf::DEFAULT_SCR_WIDTH, WindowConf::DEFAULT_SCR_HEIGHT, "´Wilkan Engine", NULL, NULL);
    if (m_window == NULL)
    {
        fmt::print("Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /*glfwSetCursorPosCallback(window, &mouseCallback);
    glfwSetScrollCallback(window, &scrollCallback);*/
    //glfwSetFramebufferSizeCallback(window, &framebufferSizeCallback);
}

bool WindowManager::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void WindowManager::swapBuffers()
{
    glfwSwapBuffers(m_window);
}
