#include "scenenode.h"
#include <glm/gtc/matrix_transform.hpp>
SceneNode::SceneNode() :
	children({}),
	position(glm::vec3(0.f)),
	scale(1.0f),
	rotations({}),
	m_model(nullptr)
{
}

void SceneNode::setModel(std::unique_ptr<Model> model)
{
	m_model = std::move(model);
}

void SceneNode::update(ShaderManager* sdrMan)
{
	if (m_model != nullptr)
		m_model->draw(createModelMatrix(), sdrMan);
}

glm::mat4 SceneNode::createModelMatrix()
{
	glm::mat4 retMat = glm::translate(glm::mat4(1.0), position);
	for (auto rot : rotations){
		glm::rotate(retMat, rot.first, rot.second);
	}
	return glm::scale(retMat, scale);
}
