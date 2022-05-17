#include "openglrenderer.h"
#include "../service_locator/locator.h"
#include <glm/gtc/type_ptr.hpp>

//#define VERBOSE

const std::unordered_map<Renderer::ImageFormat, GLenum> OpenGLRenderer::m_imgFormatMap = { {ImageFormat::RGB, GL_RGB}, {ImageFormat::RGBA, GL_RGBA}, {ImageFormat::R, GL_RED} };
OpenGLRenderer::OpenGLRenderer():
    m_meshBufferMap({}),
    m_shaderProgMap({}),
    m_texIdMap({})
{
    
}

bool OpenGLRenderer::initialize(WindowManager* windowMan)
{
    // glad: load all OpenGL function pointers
// ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)windowMan->getProcAddress()))
    {
        Locator::getLogger()->getLogger()->info("Failed to initialize GLAD\n");
        return true;
    }
    
    glClearColor(0.f, 0.f, 0.f, 1.0f);

    // Initial depth test config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /*
    * This stencil test config is wrong
    */
    // Initial stencil test config
    /*glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/
    return false;
}

void OpenGLRenderer::setViewportSize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGLRenderer::meshBuffersGenerate(int meshId)
{
    if (m_meshBufferMap.find(meshId) != m_meshBufferMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: mesh buffer gen failed: buffers already exist\n");
        return;
    }
    MeshBufferData data;
    glGenVertexArrays(1, &data.VAO);
    glGenBuffers(1,      &data.VBO);
    glGenBuffers(1,      &data.EBO);
    m_meshBufferMap.insert(std::pair<int, MeshBufferData>(meshId, data));
}

void OpenGLRenderer::meshBuffersPushData(int meshId,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
{
    auto it = m_meshBufferMap.find(meshId);
    if (it == m_meshBufferMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: mesh buffer data push failed: unknown meshId\n");
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
    auto it = m_meshBufferMap.find(meshId);
    if (it == m_meshBufferMap.end())
        return

    glDeleteVertexArrays(1, &(it->second.VAO));
    glDeleteBuffers(1, &(it->second.VBO));
    glDeleteBuffers(1, &(it->second.EBO));

    m_meshBufferMap.erase(it);
}

void OpenGLRenderer::createShaderProgram(const unsigned int &id,const char* vertexShader, const char* fragShader)
{
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    checkShaderCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragShader, NULL);
    glCompileShader(fragment);
    checkShaderCompileErrors(fragment, "FRAGMENT");
    // shader Program
    GLuint progid = glCreateProgram();
    glAttachShader(progid, vertex);
    glAttachShader(progid, fragment);
    glLinkProgram(progid);
    checkShaderCompileErrors(progid, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Id mapping
    m_shaderProgMap.insert(std::pair<unsigned int, GLuint>(id, progid));
}

void OpenGLRenderer::useShaderProgram(const unsigned int& id)
{
    auto it = m_shaderProgMap.find(id);
    if (it == m_shaderProgMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: cant use shader program: unknown program id\n");
        return;
    }
    glUseProgram(it->second);
}

void OpenGLRenderer::setBool(const unsigned int& sdrId, const char* name, bool value)
{
    shader_prog_map::iterator it;
    if (findInshaderProgMap(sdrId, it)) {
        glUniform1i(glGetUniformLocation(it->second, name), (int)value);
#ifdef VERBOSE
        Locator::getLogger()->getLogger()->info("set uniform {} in location {}", name, glGetUniformLocation(it->second, name));
#endif // VERBOSE
    }
}

void OpenGLRenderer::setInt(const unsigned int& sdrId, const char* name, int value)
{
    shader_prog_map::iterator it;
    if (findInshaderProgMap(sdrId, it)) {
        glUniform1i(glGetUniformLocation(it->second, name), value);
#ifdef VERBOSE
        Locator::getLogger()->getLogger()->info("set uniform {} in location {}", name, glGetUniformLocation(it->second, name));
#endif // VERBOSE
    }
}

void OpenGLRenderer::setuInt(const unsigned int& sdrId, const char* name, unsigned int value)
{
    shader_prog_map::iterator it;
    if (findInshaderProgMap(sdrId, it)) {
        glUniform1ui(glGetUniformLocation(it->second, name), value);
#ifdef VERBOSE
        Locator::getLogger()->getLogger()->info("set uniform {} in location {}", name, glGetUniformLocation(it->second, name));
#endif // VERBOSE
    }
}

void OpenGLRenderer::setFloat(const unsigned int& sdrId, const char* name, float value)
{
    shader_prog_map::iterator it;
    if (findInshaderProgMap(sdrId, it)) {
        glUniform1f(glGetUniformLocation(it->second, name), value);
#ifdef VERBOSE
        Locator::getLogger()->getLogger()->info("set uniform {} in location {}", name, glGetUniformLocation(it->second, name));
#endif // VERBOSE
    }
}

void OpenGLRenderer::setFloat3(const unsigned int& sdrId, const char* name, glm::vec3 value)
{
    shader_prog_map::iterator it;
    if (findInshaderProgMap(sdrId, it)) {
        glUniform3f(glGetUniformLocation(it->second, name), value.r, value.g, value.b);
#ifdef VERBOSE
        Locator::getLogger()->getLogger()->info("set uniform {} in location {}", name, glGetUniformLocation(it->second, name));
#endif // VERBOSE
    }
}

void OpenGLRenderer::setMat4f(const unsigned int& sdrId, const char* name, glm::mat4 value)
{
    shader_prog_map::iterator it;
    if (findInshaderProgMap(sdrId, it)) {
        glUniformMatrix4fv(glGetUniformLocation(it->second, name), 1, GL_FALSE, glm::value_ptr(value));
#ifdef VERBOSE
        Locator::getLogger()->getLogger()->info("set uniform {} in location {}", name, glGetUniformLocation(it->second, name));
#endif // VERBOSE
    }
}

void OpenGLRenderer::create2DTexture(const unsigned int& id, const unsigned int& width, const unsigned int& height,
    ImageFormat formatSrc, ImageFormat formatInternal, unsigned char* imgData)
{
    auto it = m_texIdMap.find(id);
    if (it != m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: can't create texture: texture id already in use\n");
        return;
    }
    GLuint glTexId;
    glGenTextures(1, &glTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexId);
    // -----------------------
    // Set texture parameters here:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*********************************
    * note: this causes an access violation exception if format is GL_RGBA and provided data is in RGB format.
    *       Also the textures may just glitch out if formats are not passed correctly...
    */
    glTexImage2D(GL_TEXTURE_2D, 0, m_imgFormatMap.at(formatInternal), width, height, 0, m_imgFormatMap.at(formatSrc), GL_UNSIGNED_BYTE, (GLvoid*)imgData);
    /*********************************/

    glBindTexture(GL_TEXTURE_2D, 0);
    m_texIdMap.insert(std::pair<unsigned int, GLuint>(id, glTexId));
}

void OpenGLRenderer::bindTo2DSampler(const unsigned int& texId, const unsigned int& samplerId)
{
    auto it = m_texIdMap.find(texId);
    if (it == m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: can't bind texture: unknown texture id\n");
        return;
    }
    glActiveTexture(GL_TEXTURE0 + samplerId);
    glBindTexture(GL_TEXTURE_2D, it->second);
}

void OpenGLRenderer::deleteTexture(const unsigned int& id)
{
    auto it = m_texIdMap.find(id);
    if (it == m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: can't delete texture: unknown texture id\n");
        return;
    }
    glDeleteTextures(1, &(it->second));
    m_texIdMap.erase(it);
}

int OpenGLRenderer::getColorBuffBit()
{
    return GL_COLOR_BUFFER_BIT;
}

int OpenGLRenderer::getStencilBuffBit()
{
    return GL_STENCIL_BUFFER_BIT;
}

int OpenGLRenderer::getDepthBuffBit()
{
    return GL_DEPTH_BUFFER_BIT;
}

void OpenGLRenderer::clearBuffer(int buffers)
{
    glClear(buffers);
}

void OpenGLRenderer::depthTesting(bool enable)
{
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderer::stencilTesting(bool enable)
{
    if (enable)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
}

void OpenGLRenderer::blending(bool enable)
{
    if (enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
        glDisable(GL_BLEND);
}

void OpenGLRenderer::drawMesh(int meshId)
{
    auto it = m_meshBufferMap.find(meshId);
    if (it == m_meshBufferMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: mesh draw failed: unknown meshId\n");
        return;
    }

    glBindVertexArray(it->second.VAO);
    glDrawElements(GL_TRIANGLES, it->second.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OpenGLRenderer::checkShaderCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            Locator::getLogger()->getLogger()->info("ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}\n -- --------------------------------------------------- -- ", type.c_str(), infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            Locator::getLogger()->getLogger()->info("ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}\n -- --------------------------------------------------- -- ", type.c_str(), infoLog);
        }
    }
}

bool OpenGLRenderer::findInshaderProgMap(const unsigned int& sdrId, shader_prog_map::iterator& it)
{
    it = m_shaderProgMap.find(sdrId);
    if (it == m_shaderProgMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: cant find shader program: unknown program id\n");
        return false;
    }
    return true;
}

