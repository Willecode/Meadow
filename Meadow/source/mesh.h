#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "asset.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords):
		position(position), normal(normal), texCoords(texCoords)
	{

	}
};

class Mesh: public Asset
{
public:
	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();
	void draw();
	void generateBuffers(); // Generate graphics memory buffers for this mesh
	void buffersPushData(); // Push vertex data and indices into previously generated buffers
	AssetType getAssetType() override;
};

