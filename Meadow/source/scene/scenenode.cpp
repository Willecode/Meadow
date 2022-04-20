#include "scenenode.h"
#include <glm/gtc/matrix_transform.hpp>
SceneNode::SceneNode(std::string name) :
	children({}),
	position(glm::vec3(0.f)),
	scale(1.0f),
	rotations({}),
	m_model(nullptr),
	m_modelMatrix(glm::mat4(1.0f)),
	name(name)
{
}

SceneNode::SceneNode(const SceneNode& n1)
{
	position = n1.position;
	scale = n1.scale;
	rotations = n1.rotations;
	m_model = n1.m_model;
	m_modelMatrix = n1.m_modelMatrix;
}

void SceneNode::setModel(std::unique_ptr<Model> model)
{
	m_model = std::move(model);
}

glm::mat4* SceneNode::getModelMatrix()
{
	return &m_modelMatrix;
}

void SceneNode::update(SceneNode* parent)
{
	updateModelMatrix(parent->getModelMatrix());
}

void SceneNode::render(ShaderManager* sdrMan)
{
	if (m_model != nullptr)
		m_model->draw(m_modelMatrix, sdrMan);
}

void SceneNode::updateModelMatrix(glm::mat4* accumulate)
{
	m_modelMatrix = glm::translate(glm::mat4(1.0), position);
	for (auto rot : rotations){
		m_modelMatrix = glm::rotate(m_modelMatrix, rot.first, rot.second);
	}
	m_modelMatrix = glm::scale(m_modelMatrix, scale);
	m_modelMatrix = (*accumulate) * m_modelMatrix;
}
