#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "shader_s.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};
struct Texture {
	unsigned int id;
	std::string type;

	Texture(unsigned int id = 0, std::string type = "") {
		this->id = id;
		this->type = type;
	}
};

class Mesh
{
public:
	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(	std::vector<Vertex>	vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);
	void draw(Shader &shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

