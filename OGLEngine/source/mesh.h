#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "shader_s.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords):
		position(position), normal(normal), texCoords(texCoords)
	{

	}
};

class Mesh
{
public:
	std::vector<Vertex>	vertices;
	std::vector<GLuint> indices;
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	~Mesh();
	void draw() const;
private:
	unsigned int VAO, VBO, EBO;
	void setup();
};

