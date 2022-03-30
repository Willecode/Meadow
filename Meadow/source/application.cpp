#include "application.h"

//--------------------------------------
// IMAGE LOADER
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
//--------------------------------------
// MATH FUNCTIONS
//#define GLM_ENABLE_EXPERIMENTAL // for glm/ext.hpp
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/ext.hpp> // For printing glm vectors
//--------------------------------------
// STRING FORMATTING
#define FMT_HEADER_ONLY
#include <fmt/core.h>
//--------------------------------------
#include "shadermanager.h"
#include "service_locator/locator.h"

Application::Application(): m_windowManager(WindowManager()), m_renderer(OpenGLRenderer()), m_scene(nullptr), appFailed(false), m_textureCache(ImageCache())
{   
    m_windowManager.createWindow("Wilkan Graphics Program");
    appFailed = m_renderer.initialize();
    ShaderManager::initShaderManager();
    Locator::provide(&m_windowManager);
    Locator::provide(&m_renderer);

    m_scene = std::make_unique<Scene>();
}

void Application::run()
{
    m_scene->updateLighting();
    m_scene->updateShaders();
    // render loop
    // -----------
    while (!m_windowManager.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_scene->drawScene();

        // glfw: swap buffers and poll IO events
        // -------------------------------------------------------------------------------
        m_windowManager.swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
}
