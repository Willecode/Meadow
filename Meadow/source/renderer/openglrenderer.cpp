#include "openglrenderer.h"
#include "glad/glad.h"
#include <fmt/format.h>
#include "../service_locator/locator.h"
bool OpenGLRenderer::initialize()
{
    // glad: load all OpenGL function pointers
// ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)Locator::getWindowMan()->getProcAddress()))
    {
        fmt::print("Failed to initialize GLAD\n");
        return true;
    }

    // Initial depth test config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Initial stencil test config
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    return false;
}

void OpenGLRenderer::setViewportSize(int width, int height)
{
    glViewport(0, 0, width, height);
}
