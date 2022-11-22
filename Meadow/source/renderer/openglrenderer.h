#pragma once
#include "renderer.h"
#include <unordered_map>
#include <glad/glad.h>
#include <glm/fwd.hpp>


class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	bool initialize(WindowManager* windowMan) override;
	void setViewportSize(int width, int height) override;

	/*
	* Mesh buffer funcs
	*/
	// Use these to load mesh into GPU memory
	void meshBuffersGenerate(int meshId) override;
	void meshBuffersPushData(int meshId,
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices) override;
	void mesh2DBuffersPushData(int meshId,
		const std::vector<Vertex2D>& vertices,
		const std::vector<unsigned int>& indices) override;
	// Use this to remove mesh from GPU memory
	void meshBuffersDelete(int meshId) override;

	// Draw a loaded mesh with currently bound textures, shader, shader uniforms
	void drawMesh(int meshId) override;

	/*
	* Shader program
	*/
	void createShaderProgram(const unsigned int& id, const char* vertexShader, const char* fragShader) override;
	void useShaderProgram(const unsigned int& id) override;
	void deleteShaderProgram(const unsigned int& id) override;
	/*
	* Shader uniform setters
	*/
	void setBool  (const unsigned int &sdrId, const char* name, bool value) override;
	void setInt   (const unsigned int &sdrId, const char* name, int value) override;
	void setuInt   (const unsigned int &sdrId, const char* name, unsigned int value) override;
	void setFloat (const unsigned int &sdrId, const char* name, float value) override;
	void setFloat3(const unsigned int &sdrId, const char* name, glm::vec3 value) override;
	void setFloat4(const unsigned int &sdrId, const char* name, glm::vec4 value) override;
	void setMat4f (const unsigned int &sdrId, const char* name, glm::mat4 value) override;
	
	/*
	* Textures
	*/
	void create2DTexture(const unsigned int&, const unsigned int& width, const unsigned int& height,
		ImageFormat formatSrc, ImageFormat formatDestination,  unsigned char* imgData, bool mipmap = true) override;
	void create2DTextureMS(const unsigned int& id, const unsigned int& width, const unsigned int& height, ImageFormat format) override;
	void bindTo2DSampler(const unsigned int& texId, const unsigned int& samplerId) override;
	void bindTo2DSamplerMS(const unsigned int& texId, const unsigned int& samplerId) override;
	void unbindTexture(const unsigned int& samplerId) override;
	void deleteTexture(const unsigned int& id) override;

	/*
	* Buffer clearing
	*/
	int getColorBuffBit() override;
	int getStencilBuffBit() override;
	int getDepthBuffBit() override;
	void clearBuffer(int buffers) override;
	void setClearColor(glm::vec4 color) override; // Set color buffer clearing color

	/*
	* Depth testing
	*/
	void depthTesting(bool enable) override;
	void depthMask(bool enable) override;

	/*
	* Stencil testing
	*/
	void stencilTesting(bool enable) override;
	void setStencilMask(unsigned int mask) override;
	void setStencilFunc(TestingFuncs func, int ref, unsigned int mask) override;
	void setStencilOp(TestingActions stencilFail, TestingActions depthFail, TestingActions depthPass) override;
	void setStencilClearValue(unsigned int) override;

	/*
	* Blending
	*/
	void blending(bool enable) override;

	/*
	* Face culling
	*/
	void faceCulling(bool enable) override;
	void cullFront() override;
	void cullBack() override;
	void cullFrontAndBack() override;
	
	/*
	* Framebuffers
	*/
	// Creates an fb with texId as color attachment, also attaches depth and stencil RBO
	void createFrameBuffer(int buffId, int texId, unsigned int width, unsigned int height) override;
	void createFrameBufferDepthMapOnly(int buffId, int texId, unsigned int width, unsigned int height) override;
	void createFrameBufferMultisample(int buffId, int texId, unsigned int width, unsigned int height) override;
	void bindFrameBuffer(int buffId) override;
	void bindFrameBufferDraw(int buffId) override;
	void bindFrameBufferRead(int buffId) override;
	void bindFrameBufferDefault() override;
	void deleteFrameBuffer(int buffId) override; // DOES NOT DELETE RBO YET!!!
	bool checkFrameBufferStatus() override;
	void getFrameBufferDimensions(int buffId, int& width, int& height) override;
	void blitFramebuffer(int width, int height) override;

	/*
	* Cubemaps
	*/
	void createCubemap(int cmId) override;
	void cubemapLoadTextures(int cmId, std::array<unsigned char*, 6> images, int width, int height) override;
	void deleteCubemap(int cmId) override;
	void bindCubemap(int cmId) override;

	/*
	* Drawing mode
	*/
	void wireframe(bool enable);

	/*
	* Multisampling toggle
	*/
	void multisampling(bool enable) override;

	

	/*
	* Disable/Enable drawing on the color buffer
	*/
	void setColorMask(bool flag) override;
private:
	struct MeshBufferData {
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint indexCount;
	};
	/*
	* Maps from user mesh id -> OpenGL buffer ids
	*/
	std::unordered_map<int, MeshBufferData> m_meshBufferMap;

	/*
	* Maps from user shader id -> OpenGL shader program id
	*/
	typedef std::unordered_map<unsigned int, GLuint> shader_prog_map;
	shader_prog_map m_shaderProgMap;

	/*
	* Maps from user texture id -> OpenGL texture id
	*/
	std::unordered_map<unsigned int, GLuint> m_texIdMap;
	
	/*
	* Maps from general renderer image format enums -> OpenGL Channel nr enums (RGB, RGB_A etc.)
	*/
	static const std::unordered_map<ImageFormat, GLenum> m_imgFormatMap;

	struct FrameBufferData {
		GLuint frameBuffId;
		GLuint rboId;
		int frameBuffWidth;
		int frameBuffHeight;
		bool multisampled;
		GLuint texId;
	};
	/*
	* Maps from user framebuffer id -> OpenGL framebuffer id
	*/
	std::unordered_map<unsigned int, FrameBufferData> m_fbIdMap;

	/*
	* Maps from user cubemap id -> OpenGL cubemap id
	*/
	std::unordered_map<unsigned int, GLuint> m_cubemapIdMap;

	/*
	* Maps from Renderer func enums to OpenGL funcs
	*/
	static const std::unordered_map<Renderer::TestingFuncs, GLuint> m_testingFuncMap;

	/*
	* Maps from Renderer action enums to OpenGL "Ops"
	*/
	static const std::unordered_map<Renderer::TestingActions, GLuint> m_actionMap;


private:

	/*
	* Utility
	*/
	void checkShaderCompileErrors(unsigned int shader, std::string type);
	bool findInshaderProgMap(const unsigned int& sdrId, shader_prog_map::iterator& it);
};

