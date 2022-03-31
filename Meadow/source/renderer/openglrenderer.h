#pragma once
#include "renderer.h"
#include <unordered_map>
#include <glad/glad.h>

class OpenGLRenderer : public Renderer
{
public:
	bool initialize() override;
	void setViewportSize(int width, int height) override;

	void meshBuffersGenerate(int meshId) override;
	void meshBuffersPushData(int meshId,
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices) override;
	void meshBuffersDelete(int meshId) override;

	void draw(int meshId) override;

	

private:
	struct MeshBufferData {
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint indexCount;
	};
	std::unordered_map<int, MeshBufferData> m_idMap;
};

