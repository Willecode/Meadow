#pragma once
#include "ecs/core/system.h"
#include "ecs/core/ecscoordinator.h"
#include <physx/PxPhysicsAPI.h>
class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
protected:
	void onEntityAdded(Entity ent) override;
	void onEntityRemoved(Entity ent) override;
private:
	void togglePhysics(bool f);
private:
	physx::PxDefaultErrorCallback  m_PxDefaultErrorCallback;
	physx::PxDefaultAllocator	   m_PxDefaultAllocator;
	physx::PxDefaultCpuDispatcher* m_PxDefaultCpuDispatcher;
	physx::PxFoundation*		   m_PxFoundation;
	physx::PxPhysics*			   m_PxPhysics;
	physx::PxPvd*				   m_PxPvd;
	physx::PxScene*				   m_PxScene;

	physx::PxMaterial* m_defaultMaterial;

	float m_timeAccumulate;
	float m_stepSize;

	bool m_physicsEnabled;
	ECSCoordinator* m_ecs;
};

