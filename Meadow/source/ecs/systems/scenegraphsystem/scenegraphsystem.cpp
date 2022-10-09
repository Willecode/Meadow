#include "scenegraphsystem.h"
#include "ecs/components/transform.h"
#include "ecs/core/internalevents.h"
#include "input/inputevents.h"
#include <glm/gtc/matrix_transform.hpp>


void SceneGraphSystem::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;
	InternalEvents::EntityCreatedEvent::subscribe(std::bind(&SceneGraphSystem::entityCreated, this, std::placeholders::_1));
	InternalEvents::EntityDeletedEvent::subscribe(std::bind(&SceneGraphSystem::entityDestroyed, this, std::placeholders::_1));
	InputEvents::SetActiveNodeEvent::subscribe(std::bind(&SceneGraphSystem::setActiveNode, this, std::placeholders::_1));
	InputEvents::SetNodeParentToRootEvent::subscribe(std::bind(&SceneGraphSystem::setEntityParentToRoot, this, std::placeholders::_1));
	InputEvents::MouseButtonLeftPressedEvent::subscribe(std::bind(&SceneGraphSystem::deactivateNodes, this));
	InputEvents::SetNodeParentEvent::subscribe(std::bind(&SceneGraphSystem::changeParent, this, std::placeholders::_1, std::placeholders::_2));
}

void SceneGraphSystem::update()
{
	SceneGraph::Node root = m_sceneGraph.getGraph();
	for (auto& child : root.children) {
		calcModelMatrices(child, glm::mat4(1.0f), glm::quat());
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

void SceneGraphSystem::calcModelMatrices(const SceneGraph::Node &node, glm::mat4 matrixAccumulated, glm::quat orientationAcc)
{
	Entity ent = node.entity;
	Transform& trans = m_ecs->getComponent<Transform>(ent);
	trans.orientation = glm::normalize(trans.orientation); // Normalize quat
	trans.worldOrientation = glm::normalize(orientationAcc * trans.orientation);
	trans.modelMatrix = glm::translate(glm::mat4(1.0), trans.position);
	trans.modelMatrix *= glm::toMat4(trans.orientation);
	trans.modelMatrix = glm::scale(trans.modelMatrix, trans.scale);
	trans.modelMatrix = matrixAccumulated * trans.modelMatrix;
	trans.worldPos = glm::vec3(trans.modelMatrix[3].x, trans.modelMatrix[3].y, trans.modelMatrix[3].z);
	for (auto& child : node.children) {
		calcModelMatrices(child, trans.modelMatrix, trans.worldOrientation);
	}
}

Entity SceneGraphSystem::getActiveNode()
{
	return m_activeNode;
}

void SceneGraphSystem::setActiveNode(Entity ent)
{
	if (ent == 0)
		m_activeNode = NullEntity;
	else
		m_activeNode = ent;
}

void SceneGraphSystem::deactivateNodes()
{
	setActiveNode(0);
}

void SceneGraphSystem::setEntityParentToRoot(Entity ent)
{
	m_sceneGraph.changeParent(ent, NullEntity);
}
