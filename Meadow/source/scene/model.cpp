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

void Model::draw(glm::mat4 modelMat, ShaderManager* sdrMan)
{
	sdrMan->setUniformDrawSpecific("model", modelMat);
	m_material->passToShader(sdrMan);
	sdrMan->forwardUniformsDrawSpecific();
	for (auto m : m_meshes) {
		m->draw();
	}
}
