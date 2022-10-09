#pragma once
#include "ecs/core/system.h"
#include "ecs/systems/scenegraphsystem/scenegraph.h"
#include "ecs/core/ecscoordinator.h"
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>


class SceneGraphSystem : public System
{
public:
	void init(ECSCoordinator* ecs);
	void update();
	const SceneGraph::Node& getSceneGraph();
	Entity getActiveNode();
private:
	ECSCoordinator* m_ecs;
	SceneGraph m_sceneGraph;
	Entity m_activeNode;
	void changeParent(Entity ent, Entity parent);
	void entityDestroyed(Entity ent);
	void entityCreated(Entity ent);
	void calcModelMatrices(const SceneGraph::Node& node, glm::mat4 matrix, glm::quat orientationAcc);

	void setActiveNode(Entity ent);
	void deactivateNodes();
	void setEntityParentToRoot(Entity ent);
};

