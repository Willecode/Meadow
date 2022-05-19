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

	// Use this to remove mesh from GPU memory
	void meshBuffersDelete(int meshId) override;

	// Draw a loaded mesh with currently bound textures, shader, shader uniforms
	void drawMesh(int meshId) override;

	/*
	* Shader program
	*/
	void createShaderProgram(const unsigned int& id, const char* vertexShader, const char* fragShader) override;
	void useShaderProgram(const unsigned int& id) override;
	
	/*
	* Shader uniform setters
	*/
	void setBool  (const unsigned int &sdrId, const char* name, bool value) override;
	void setInt   (const unsigned int &sdrId, const char* name, int value) override;
	void setuInt   (const unsigned int &sdrId, const char* name, unsigned int value) override;
	void setFloat (const unsigned int &sdrId, const char* name, float value) override;
	void setFloat3(const unsigned int &sdrId, const char* name, glm::vec3 value) override;
	void setMat4f (const unsigned int &sdrId, const char* name, glm::mat4 value) override;
	
	/*
	* Textures
	*/
	void create2DTexture(const unsigned int&, const unsigned int& width, const unsigned int& height,
		ImageFormat formatSrc, ImageFormat formatInternal,  unsigned char* imgData) override;
	void bindTo2DSampler(const unsigned int& texId, const unsigned int& samplerId) override;
	void unbindTexture(const unsigned int& samplerId) override;
	void deleteTexture(const unsigned int& id) override;

	/*
	* Buffer clearing
	*/
	int getColorBuffBit() override;
	int getStencilBuffBit() override;
	int getDepthBuffBit() override;
	void clearBuffer(int buffers) override;

	/*
	* Depth testing
	*/
	void depthTesting(bool enable) override;

	/*
	* Stencil testing
	*/
	void stencilTesting(bool enable) override;

	/*
	* Blending
	*/
	void blending(bool enable) override;

	/*
	* Face culling
	*/
	void faceCulling(bool enable) override;

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

private:

	/*
	* Utility
	*/
	void checkShaderCompileErrors(unsigned int shader, std::string type);
	bool findInshaderProgMap(const unsigned int& sdrId, shader_prog_map::iterator& it);
};

