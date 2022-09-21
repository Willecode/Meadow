#pragma once
#include "ecs/core/system.h"
#include "ecs/core/scenegraph.h"
#include "ecs/core/ecscoordinator.h"
#include <glm/mat4x4.hpp>


class SceneGraphSystem : public System
{
public:
	void init();
	void update(ECSCoordinator& ecs);
	const SceneGraph::Node& getSceneGraph();
private:
	SceneGraph m_sceneGraph;
	void changeParent(Entity ent, Entity parent);
	void entityDestroyed(Entity ent);
	void entityCreated(Entity ent);
	void calcModelMatrices(const SceneGraph::Node& node, glm::mat4 matrix, ECSCoordinator& ecs);
};

