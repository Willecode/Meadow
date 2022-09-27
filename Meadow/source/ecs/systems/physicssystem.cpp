#include "PhysicsSystem.h"

using namespace physx;
void PhysicsSystem::init()
{
	m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxDefaultAllocator, m_PxDefaultErrorCallback);
	m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true);
	if (!m_PxPhysics) {
		throw("physics init failed");
	}

	// Create PhysX scene
	PxSceneDesc sceneDesc(m_PxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	// Multithreading
	//m_PxDefaultCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
	//sceneDesc.cpuDispatcher = m_PxDefaultCpuDispatcher;
	 
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//m_PxScene = m_PxPhysics->createScene(sceneDesc);

	// Call release() to delete objects

	m_defaultMaterial = m_PxPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// Add a box
	PxShape* shape = m_PxPhysics->createShape(PxBoxGeometry(1, 1, 1), *m_defaultMaterial);
	PxTransform localTm(0,1,0);
	PxRigidDynamic* body = m_PxPhysics->createRigidDynamic(localTm);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	m_PxScene->addActor(*body);
	shape->release();

	// Add a ground plane
	PxRigidStatic* groundPlane = PxCreatePlane(*m_PxPhysics, PxPlane(0, 1, 0, 0), *m_defaultMaterial);
	m_PxScene->addActor(*groundPlane);

	m_timeAccumulate = 0.0f;
	m_stepSize = 1.0f / 60.0f;

}

void PhysicsSystem::update(float deltaT, ECSCoordinator& ecs)
{
	m_timeAccumulate += deltaT;
	if (m_timeAccumulate < m_stepSize)
		return;

	m_timeAccumulate -= m_stepSize;

	m_PxScene->simulate(m_stepSize);
	m_PxScene->fetchResults(true);
}
