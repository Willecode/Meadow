#include "scenenode.h"
#include <glm/gtc/matrix_transform.hpp>
SceneNode::SceneNode(unsigned int id, std::string name) :
	children({}),
	position(glm::vec3(0.f)),
	scale(1.0f),
	rotations({}),
	m_mesh(nullptr),
	m_light(nullptr),
	m_modelMatrix(glm::mat4(1.0f)),
	name(name),
	id(id)
{
}

SceneNode::SceneNode(const SceneNode& n1)
{
	position = n1.position;
	scale = n1.scale;
	rotations = n1.rotations;
	m_mesh = n1.m_mesh;
	m_modelMatrix = n1.m_modelMatrix;
}

SceneNode& SceneNode::operator=(const SceneNode& n1)
{
	position = n1.position;
	scale = n1.scale;
	rotations = n1.rotations;
	m_mesh = n1.m_mesh;
	m_modelMatrix = n1.m_modelMatrix;
	return *this;
}

void SceneNode::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

Mesh* SceneNode::getMesh()
{
	return m_mesh;
}

void SceneNode::setLightSource(std::unique_ptr<LightSource> ls)
{
	auto oldLight = std::move(m_light);
	m_light = std::move(ls);
}

LightSource* SceneNode::getLightsource()
{
	return m_light.get();
}

glm::mat4* SceneNode::getModelMatrix()
{
	return &m_modelMatrix;
}

void SceneNode::update(SceneNode* parent)
{
	updateModelMatrix(parent->getModelMatrix());

	/*
	* Update the lightsource's position, !!!THIS IS UNINTUITIVE!!!!,
	* to accumulate transform take the pos from the modelmatrix
	*/
	if (m_light != nullptr)
		m_light->setProperty(LightSource::PropertyType::POSITION, m_modelMatrix[3]);
}

void SceneNode::render(ShaderManager* sdrMan)
{
	if (m_mesh != nullptr)
		m_mesh->draw(m_modelMatrix, sdrMan);
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
