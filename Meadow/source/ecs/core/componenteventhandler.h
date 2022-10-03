#pragma once
#include "ecscoordinator.h"
#include "ecs/components/rigidbody.h"
class ComponentEventHandler
{
public:
	void init(ECSCoordinator* ecs);
private:
	ECSCoordinator* m_ecs;
private:
	void add3DModelEventHandler(Entity ent);
	void remove3DModelEventHandler(Entity ent);
	void addLightsourceEventHandler(Entity ent);
	void removeLightsourceEventHandler(Entity ent);
	void addRigidBodyEventHandler(Entity ent, RigidBody::RigidBodyType t);
	void removeRigidBodyEventHandler(Entity ent);

	void setNodeMeshHandler(Entity ent, unsigned int meshid);
};

