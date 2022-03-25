#include "application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//--------------------------------------
// IMAGE LOADER
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
//--------------------------------------
// MATH FUNCTIONS
#define GLM_ENABLE_EXPERIMENTAL // for glm/ext.hpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp> // For printing glm vectors
//--------------------------------------
// STRING FORMATTING
#define FMT_HEADER_ONLY
#include <fmt/core.h>
//--------------------------------------
#include "shader_s.h"
#include "scene.h"
#include "object3d.h"
#include "texture.h"
#include "materials/phongmaterial.h"
#include "materials/coloronlymaterial.h"
#include "primitivecreation.h"
#include "modelimporting.h"
#include "windowmanager.h"
#include "shadermanager.h"

#include <iostream>
#include <map>
#include <array>
#include <vector>

bool setupOpenGL() {
    // glad: load all OpenGL function pointers
// ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::print("Failed to initialize GLAD\n");
        return false;
    }

    // Initial depth test config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Initial stencil test config
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    return true;
}

Application::Application(): m_windowManager(WindowManager()), m_scene(nullptr), appFailed(false), m_textureCache(ImageCache())
{   
    m_windowManager.createWindow();
    appFailed = !setupOpenGL();
    ShaderManager::initShaderManager();
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
