#include "cube.h"

Cube::Cube()
{
	Vertex v1, v2, v3;
	v1.position = glm::vec3(0.0f);
	v1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	v1.texCoords = glm::vec2(0.0f);

	v2.position = glm::vec3(1.0f, 1.0f, 0.0f);
	v2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	v2.texCoords = glm::vec2(1.0f);

	v3.position = glm::vec3(0.0f, 1.0f, 0.0f);
	v3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	v3.texCoords = glm::vec2(0.0f, 1.0f);

	std::vector<Vertex> verts;
	verts.push_back(v1);
	verts.push_back(v2);
	verts.push_back(v3);

	std::vector<GLuint> indices = { 0, 1, 2 };

	Mesh mesh(verts, indices);
	meshes.push_back(mesh);

}

void Cube::scale(int amount)
{
}
