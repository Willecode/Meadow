#include "openglrenderer.h"
#include "../service_locator/locator.h"
#include <glm/gtc/type_ptr.hpp>

//#define VERBOSE

const std::unordered_map<Renderer::ImageFormat, GLenum> OpenGLRenderer::m_imgFormatMap = 
{
    {ImageFormat::RGB, GL_RGB},
    {ImageFormat::RGB_HIGH_PRECISION, GL_RGB32F},
    {ImageFormat::RGBA, GL_RGBA},
    {ImageFormat::sRGB, GL_SRGB},
    {ImageFormat::sRGBA, GL_SRGB_ALPHA},
    {ImageFormat::R, GL_RED} };

const std::unordered_map<Renderer::TestingFuncs, GLuint> OpenGLRenderer::m_testingFuncMap =
{
    {Renderer::TestingFuncs::NEVER, GL_NEVER},
    {Renderer::TestingFuncs::LESS, GL_LESS},
    {Renderer::TestingFuncs::LEQUAL, GL_LEQUAL},
    {Renderer::TestingFuncs::GREATER, GL_GREATER},
    {Renderer::TestingFuncs::GEQUAL, GL_GEQUAL},
    {Renderer::TestingFuncs::EQUAL, GL_EQUAL},
    {Renderer::TestingFuncs::NOTEQUAL, GL_NOTEQUAL},
    {Renderer::TestingFuncs::ALWAYS, GL_ALWAYS}
};

const std::unordered_map<Renderer::TestingActions, GLuint> OpenGLRenderer::m_actionMap =
{
    {Renderer::TestingActions::KEEP, GL_KEEP},
    {Renderer::TestingActions::ZERO, GL_ZERO},
    {Renderer::TestingActions::REPLACE, GL_REPLACE},
    {Renderer::TestingActions::INCR, GL_INCR},
    {Renderer::TestingActions::INCR_WRAP, GL_INCR_WRAP},
    {Renderer::TestingActions::DECR, GL_DECR},
    {Renderer::TestingActions::DECR_WRAP, GL_DECR_WRAP},
    {Renderer::TestingActions::INVERT, GL_INVERT},

};

OpenGLRenderer::OpenGLRenderer():
    m_meshBufferMap({}),
    m_shaderProgMap({}),
    m_texIdMap({}),
    m_fbIdMap({})
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
    glEnable(GL_MULTISAMPLE);
    faceCulling(false);
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

void OpenGLRenderer::mesh2DBuffersPushData(int meshId, const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices)
{
    auto it = m_meshBufferMap.find(meshId);
    if (it == m_meshBufferMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: mesh buffer data push failed: unknown meshId\n");
        return;
    }

    it->second.indexCount = static_cast<unsigned int>(indices.size());

    glBindVertexArray(it->second.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, it->second.VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));

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
    // check if program already exists
    auto it = m_shaderProgMap.find(id);
    if (it != m_shaderProgMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: cant create shader program: id taken\n");
        return;
    }

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

void OpenGLRenderer::deleteShaderProgram(const unsigned int& id)
{
    auto it = m_shaderProgMap.find(id);
    if (it == m_shaderProgMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: cant delete shader program: unknown program id\n");
        return;
    }
    glDeleteProgram(it->second);
    m_shaderProgMap.erase(it);
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
    ImageFormat formatSrc, ImageFormat formatDestination, unsigned char* imgData)
{
    auto it = m_texIdMap.find(id);
    if (it != m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: can't create texture: texture id already in use\n");
        return;
    }
    GLuint glTexId;
    glGenTextures(1, &glTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexId);
    // -----------------------
    // Set texture parameters here:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /*********************************
    * note: this causes an access violation exception if format is GL_RGBA and provided data is in RGB format.
    *       Also the textures may just glitch out if formats are not passed correctly...
    */
    if (imgData == nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, m_imgFormatMap.at(formatDestination), width, height, 0,
            m_imgFormatMap.at(formatSrc), GL_UNSIGNED_BYTE, NULL);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, m_imgFormatMap.at(formatDestination), width, height, 0,
            m_imgFormatMap.at(formatSrc), GL_UNSIGNED_BYTE, (GLvoid*)imgData);
    }
    /*********************************/

    glBindTexture(GL_TEXTURE_2D, 0);
    m_texIdMap.insert(std::pair<unsigned int, GLuint>(id, glTexId));
}

void OpenGLRenderer::create2DTextureMS(const unsigned int& id, const unsigned int& width, const unsigned int& height, ImageFormat format)
{
    auto it = m_texIdMap.find(id);
    if (it != m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: can't create texture: texture id already in use\n");
        return;
    }
    GLuint glTexId;
    glGenTextures(1, &glTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glTexId);

    // Set multisample
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, m_imgFormatMap.at(format), width, height, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
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
    glActiveTexture(GL_TEXTURE0);
}

void OpenGLRenderer::bindTo2DSamplerMS(const unsigned int& texId, const unsigned int& samplerId)
{
    auto it = m_texIdMap.find(texId);
    if (it == m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->info("Renderer: can't bind MS texture: unknown texture id\n");
        return;
    }
    glActiveTexture(GL_TEXTURE0 + samplerId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, it->second);
    glActiveTexture(GL_TEXTURE0);
}

void OpenGLRenderer::unbindTexture(const unsigned int& samplerId)
{
    glActiveTexture(GL_TEXTURE0 + samplerId);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
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

void OpenGLRenderer::setClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderer::depthTesting(bool enable)
{
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderer::depthMask(bool enable)
{
    if (enable)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);
}

void OpenGLRenderer::stencilTesting(bool enable)
{
    if (enable)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
}

void OpenGLRenderer::setStencilMask(unsigned int mask)
{
    if (mask == 0) {
        glStencilMask(0x00);
    }
    else
    {
        glStencilMask(0xFF);
    }
}

void OpenGLRenderer::setStencilFunc(TestingFuncs func, int ref, unsigned int mask)
{
    glStencilFunc(m_testingFuncMap.at(func), ref, mask);
}

void OpenGLRenderer::setStencilOp(TestingActions stencilFail, TestingActions depthFail, TestingActions depthPass)
{
    glStencilOp(m_actionMap.at(stencilFail), m_actionMap.at(depthFail), m_actionMap.at(depthPass));
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

void OpenGLRenderer::faceCulling(bool enable)
{
    if (enable) {
        glEnable(GL_CULL_FACE);
    }
    else
        glDisable(GL_CULL_FACE);
}

void OpenGLRenderer::createFrameBuffer(int buffId, int texId, unsigned int width, unsigned int height)
{
    // Check fb already taken
    auto it = m_fbIdMap.find(buffId);
    if (it != m_fbIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: framebuffer creation failed: framebuffer with id already exists\n");
        return;
    }
    
    // Check that tex exists
    auto it2 = m_texIdMap.find(texId);
    if (it2 == m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: framebuffer creation failed: nonexistent texid provided\n");
        return;
    }
    GLuint oglTexId = it2->second;

    // Create opengl fb
    unsigned int glbuffId;
    glGenFramebuffers(1, &glbuffId);
    glBindFramebuffer(GL_FRAMEBUFFER, glbuffId);

    // Attach texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, oglTexId, 0);

    // Attach rbo as a depth/stencil buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    FrameBufferData fbData = {
        glbuffId, // Framebuffer Id
        rbo, // RBO id
        width,
        height,
        false, // Multisampled?
        0
    };
    m_fbIdMap.insert(std::pair<unsigned int, FrameBufferData>(buffId, fbData));
}

void OpenGLRenderer::createFrameBufferMultisample(int buffId, int texId, unsigned int width, unsigned int height)
{
    // Check fb already taken
    auto it = m_fbIdMap.find(buffId);
    if (it != m_fbIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: MS framebuffer creation failed: framebuffer with id already exists\n");
        return;
    }
    // Check that tex exists
    auto it2 = m_texIdMap.find(texId);
    if (it2 == m_texIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: MS framebuffer creation failed: nonexistent texid provided\n");
        return;
    }
    GLuint oglTexId = it2->second;

    ///////////////////////////////
    // Create multisample framebuffer
    ///////////////////////////////

    // Create opengl fb
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Attach texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, oglTexId, 0);

    // Attach rbo as a depth/stencil buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    FrameBufferData fbData = {
        fbo, // Framebuffer Id
        rbo, // RBO id
        width,
        height,
        true, // Multisampled?
        oglTexId
    };
    m_fbIdMap.insert(std::pair<unsigned int, FrameBufferData>(buffId, fbData));
}

void OpenGLRenderer::bindFrameBuffer(int buffId)
{
    auto it = m_fbIdMap.find(buffId);
    if (it == m_fbIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: framebuffer can't be bound: nonexistent fb id\n");
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, it->second.frameBuffId);
}

void OpenGLRenderer::bindFrameBufferDraw(int buffId)
{
    auto it = m_fbIdMap.find(buffId);
    if (it == m_fbIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: framebuffer can't be bound: nonexistent fb id\n");
        return;
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, it->second.frameBuffId);
}

void OpenGLRenderer::bindFrameBufferRead(int buffId)
{
    auto it = m_fbIdMap.find(buffId);
    if (it == m_fbIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: framebuffer can't be bound: nonexistent fb id\n");
        return;
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, it->second.frameBuffId);
}

void OpenGLRenderer::bindFrameBufferDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::deleteFrameBuffer(int buffId)
{
}

bool OpenGLRenderer::checkFrameBufferStatus()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    return true;
}

void OpenGLRenderer::getFrameBufferDimensions(int buffId, int& width, int& height)
{
    auto it = m_fbIdMap.find(buffId);
    if (it == m_fbIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: framebuffer can't be bound: nonexistent fb id\n");
        return;
    }
    width = it->second.frameBuffWidth;
    height = it->second.frameBuffHeight;
}

void OpenGLRenderer::blitFramebuffer(int width, int height)
{
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


void OpenGLRenderer::createCubemap(int cmId)
{
    // Check id already taken
    auto it = m_cubemapIdMap.find(cmId);
    if (it != m_cubemapIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: cubemap creation failed: cubemap with id already exists\n");
        return;
    }
    // Generate cubemap
    unsigned int cubemapId;
    glGenTextures(1, &cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

    // store into map
    m_cubemapIdMap.insert(std::make_pair(cmId, cubemapId));

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void OpenGLRenderer::cubemapLoadTextures(int cmId, std::array<unsigned char*, 6> images, int width, int height)
{
    // Check id exists
    auto it = m_cubemapIdMap.find(cmId);
    if (it == m_cubemapIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: cubemap can't be loaded failed: id doesn't exist\n");
        return;
    }

    // Bind
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapIdMap.at(cmId));

    // Load
    for (int i = 0; i < images.size(); i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]
        );
    }
    // Set tex parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void OpenGLRenderer::deleteCubemap(int cmId)
{
    // Check id exists
    auto it = m_cubemapIdMap.find(cmId);
    if (it == m_cubemapIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: cubemap can't be deleted: id doesn't exist\n");
        return;
    }
    glDeleteTextures(0, &it->second);
}

void OpenGLRenderer::bindCubemap(int cmId)
{
    // Check id exists
    auto it = m_cubemapIdMap.find(cmId);
    if (it == m_cubemapIdMap.end()) {
        Locator::getLogger()->getLogger()->error("Renderer: cubemap can't be bound: id doesn't exist\n");
        return;
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, it->second);
}

void OpenGLRenderer::wireframe(bool enable)
{
    if (enable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
        
}

void OpenGLRenderer::multisampling(bool enable)
{
    if (enable)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);
}

void OpenGLRenderer::setColorMask(bool flag)
{
    if (flag)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    else
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
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

