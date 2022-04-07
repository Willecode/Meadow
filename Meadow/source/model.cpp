#include "model.h"

Model::Model() :
	m_meshes(),
	m_material(nullptr)
{
}

void Model::addMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
}

void Model::setMaterial(Material* material)
{
	m_material = material;
}

void Model::draw(glm::mat4 modelMat)
{
	m_material->passToRenderer();
	for (auto m : m_meshes) {
		m->draw();
	}
}
