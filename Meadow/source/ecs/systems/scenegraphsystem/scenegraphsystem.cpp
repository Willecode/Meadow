#include "scenegraphsystem.h"
#include "ecs/components/transform.h"
#include "ecs/core/internalevents.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

void SceneGraphSystem::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;
	InternalEvents::EntityCreatedEvent::subscribe(std::bind(&SceneGraphSystem::entityCreated, this, std::placeholders::_1));
	InternalEvents::EntityDeletedEvent::subscribe(std::bind(&SceneGraphSystem::entityDestroyed, this, std::placeholders::_1));
}

void SceneGraphSystem::update()
{
	SceneGraph::Node root = m_sceneGraph.getGraph();
	for (auto& child : root.children) {
		calcModelMatrices(child, glm::mat4(1.0f));
	}
}

const SceneGraph::Node& SceneGraphSystem::getSceneGraph()
{
	return m_sceneGraph.getGraph();
}

void SceneGraphSystem::changeParent(Entity ent, Entity parent)
{
	m_sceneGraph.changeParent(ent, parent);
}

void SceneGraphSystem::entityDestroyed(Entity ent)
{
	m_sceneGraph.deleteNode(ent);
}

void SceneGraphSystem::entityCreated(Entity ent)
{
	m_sceneGraph.addNode(ent);
}

void SceneGraphSystem::calcModelMatrices(const SceneGraph::Node &node, glm::mat4 matrixAccumulated)
{
	Entity ent = node.entity;
	Transform& trans = m_ecs->getComponent<Transform>(ent);
	//trans.orientation = glm::quat(trans.orientationEuler);
	trans.modelMatrix = glm::translate(glm::mat4(1.0), trans.position);
	trans.modelMatrix *= glm::toMat4(trans.orientation);
	trans.modelMatrix = glm::scale(trans.modelMatrix, trans.scale);
	trans.modelMatrix = matrixAccumulated * trans.modelMatrix;

	for (auto& child : node.children) {
		calcModelMatrices(child, matrixAccumulated);
	}
}
