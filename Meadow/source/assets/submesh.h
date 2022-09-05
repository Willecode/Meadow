#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "asset.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	Vertex(
		glm::vec3 position,
		glm::vec3 normal,
		glm::vec2 texCoords,
		glm::vec3 tangent   = glm::vec3(1.0f), // init these to zero until primitivecreation.cpp implements these 
		glm::vec3 bitangent = glm::vec3(1.0f)):
		position(position), normal(normal), texCoords(texCoords), tangent(tangent), bitangent(bitangent)
	{

	}
};

class SubMesh: public Asset
{
public:
	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;
	SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string name = "Nameless mesh");
	void draw();
	void generateBuffers(); // Generate graphics memory buffers for this mesh
	void buffersPushData(); // Push vertex data and indices into previously generated buffers
	void deleteBuffers();   // Free the graphics memory buffers this mesh is occupying
	AssetType getAssetType() override;
};

