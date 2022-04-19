#include "windowmanager.h"
#include "service_locator/locator.h"

namespace WindowConf {
    const float DEFAULT_SCR_WIDTH = 1920.0f;
    const float DEFAULT_SCR_HEIGHT = 1080.0f;
}
float WindowManager::width = WindowConf::DEFAULT_SCR_WIDTH;
float WindowManager::height = WindowConf::DEFAULT_SCR_HEIGHT;
WindowManager::WindowManager(): m_window(nullptr), m_mouseLock(false)
{
}

WindowManager::~WindowManager()
{
    glfwTerminate();
    glfwDestroyWindow(m_window);
}

bool WindowManager::createWindow(std::string title, Dispatcher* disp)
{
    // glfw: initialize and configure
// ------------------------------
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
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
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);

    /*
    * Lock cursor to screen
    */
    if (m_mouseLock) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    /*
    * Window resize callback
    */
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

    /*
    * Subscribe to windowclose event
    */
    std::function<void(const char*)> f = std::bind(&WindowManager::closeWindowEventHandler, this, std::placeholders::_1);
    disp->subscribe(CloseWindowEvent::EVENT_TYPE, f);

    /*
    * Subscribe to mouse lock/unlock event
    */
    std::function<void(const char*)> fml = std::bind(&WindowManager::toggleMouseLockHandler, this, std::placeholders::_1);
    disp->subscribe(ToggleMouseLockEvent::EVENT_TYPE, fml);

    /*
    * Initialization succesful
    */
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

void WindowManager::errorCallback(int error, const char* description)
{
    Locator::getLogger()->getLogger()->error("WindowManager: glfw error: {} {}", error, description);
}

void WindowManager::closeWindowEventHandler(const char* eventType)
{
    glfwSetWindowShouldClose(m_window, true);
}

void WindowManager::toggleMouseLockHandler(const char* eventType)
{
    /*
    * Lock cursor to screen
    */
    if (m_mouseLock) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    /*
   * Unlock cursor
   */
    if (!m_mouseLock) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    m_mouseLock = !m_mouseLock;
}
