#pragma once
#include "ecs/core/system.h"
#include "ecs/core/ecscoordinator.h"
#include <physx/PxPhysicsAPI.h>
class PhysicsSystem : public System
{
public:
	void init();
	void update(float deltaT, ECSCoordinator& ecs);
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
};

