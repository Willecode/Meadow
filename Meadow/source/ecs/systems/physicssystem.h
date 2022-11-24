#pragma once
#include "ecs/core/system.h"
#include "ecs/core/ecscoordinator.h"
#include <physx/PxPhysicsAPI.h>
#include "collisioncallback.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
	void drawColliders();
protected:
	void onEntityAdded(Entity ent) override;
	void onEntityRemoved(Entity ent) override;
private:
	void physicsOn();
	void physicsOff();
	void toggleColliderVisibility(bool f);
private:
	physx::PxDefaultErrorCallback  m_PxDefaultErrorCallback;
	physx::PxDefaultAllocator	   m_PxDefaultAllocator;
	physx::PxDefaultCpuDispatcher* m_PxDefaultCpuDispatcher;
	physx::PxFoundation*		   m_PxFoundation;
	physx::PxPhysics*			   m_PxPhysics;
	physx::PxPvd*				   m_PxPvd;
	physx::PxScene*				   m_PxScene;
	physx::PxCooking*			   m_PxCooking;
	physx::PxCudaContextManager*   m_cudaContextManager;
	CollisionCallback			   m_collisionCallback;

	physx::PxMaterial* m_defaultMaterial;

	float m_timeAccumulate;
	float m_stepSize;

	unsigned int m_boxColliderMesh;
	unsigned int m_sphereColliderMesh;

	bool m_physicsEnabled;
	bool m_visibleColliders;
	ECSCoordinator* m_ecs;
};

