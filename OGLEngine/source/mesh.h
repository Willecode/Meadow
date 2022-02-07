#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader_s.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoords;
};
struct Texture {
	unsigned int id;
	std::string type;
};
class Mesh
{
public:
	const std::vector<Vertex>		&vertices;
	const std::vector<unsigned int> &indices;
	const std::vector<Texture>	    &textures;

	Mesh(	const std::vector<Vertex>		vertices,
			const std::vector<unsigned int> indices,
			const std::vector<Texture>		textures);
	void draw(Shader &shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

