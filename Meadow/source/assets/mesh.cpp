#include "mesh.h"

Mesh::Mesh() :
	meshes(),
	material(nullptr)
{
}

void Mesh::draw(glm::mat4 modelMat, ShaderManager* sdrMan)
{
	sdrMan->setUniformDrawSpecific("model", modelMat);
	material->passToShader(sdrMan);
	sdrMan->forwardUniformsDrawSpecific();
	for (auto m : meshes) {
		m->draw();
	}
}