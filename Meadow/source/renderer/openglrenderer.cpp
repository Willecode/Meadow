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

void OpenGLRenderer::meshBuffersGenerate(int meshId)
{
    if (m_idMap.find(meshId) != m_idMap.end()) {
        fmt::print("Renderer: mesh buffer gen failed: buffers already exist\n");
        return;
    }
    MeshBufferData data;
    glGenVertexArrays(1, &data.VAO);
    glGenBuffers(1,      &data.VBO);
    glGenBuffers(1,      &data.EBO);
    m_idMap.insert(std::pair<int, MeshBufferData>(meshId, data));
}

void OpenGLRenderer::meshBuffersPushData(int meshId,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
{
    auto it = m_idMap.find(meshId);
    if (it == m_idMap.end()) {
        fmt::print("Renderer: mesh buffer data push failed: meshid not known\n");
            return;
    }

    it->second.indexCount = static_cast<unsigned int>(indices.size());

    glBindVertexArray(it->second.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, it->second.VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void OpenGLRenderer::meshBuffersDelete(int meshId)
{
    auto it = m_idMap.find(meshId);
    if (it == m_idMap.end())
        return

    glDeleteVertexArrays(1, &(it->second.VAO));
    glDeleteBuffers(1, &(it->second.VBO));
    glDeleteBuffers(1, &(it->second.EBO));
}

void OpenGLRenderer::draw(int meshId)
{
    auto it = m_idMap.find(meshId);
    if (it == m_idMap.end()) {
        fmt::print("Renderer: mesh draw failed: meshid not known\n");
        return;
    }

    glBindVertexArray(it->second.VAO);
    glDrawElements(GL_TRIANGLES, it->second.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
