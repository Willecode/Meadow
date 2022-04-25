#include "model.h"

Model::Model() :
	meshes(),
	material(nullptr)
{
}

void Model::draw(glm::mat4 modelMat, ShaderManager* sdrMan)
{
	sdrMan->setUniformDrawSpecific("model", modelMat);
	material->passToShader(sdrMan);
	sdrMan->forwardUniformsDrawSpecific();
	for (auto m : meshes) {
		m->draw();
	}
}