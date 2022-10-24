#include "PhysicsSystem.h"
#include "input/inputevents.h"
#include "ecs/components/transform.h"
#include "ecs/components/rigidbody.h"
#include "ecs/components/model3d.h"
#include "resource_management/resourcemanager.h"
#include "shader/shadermanager.h"
#include "utils/primitivecreation.h"
#include <physx/extensions/PxRigidActorExt.h>
using namespace physx;
PhysicsSystem::PhysicsSystem() : m_physicsEnabled(false), m_visibleColliders(false)
{
	InputEvents::PlayGameEvent::subscribe(std::bind(&PhysicsSystem::physicsOn, this));
	InputEvents::StopGameEvent::subscribe(std::bind(&PhysicsSystem::physicsOff, this));
	InputEvents::ColliderVisibilityEvent::subscribe(std::bind(&PhysicsSystem::toggleColliderVisibility, this, std::placeholders::_1));
}
PhysicsSystem::~PhysicsSystem()
{
	PxCloseExtensions();
	//m_defaultMaterial->release();
	m_PxPvd->release();
	//m_PxScene->release();
	//m_PxDefaultCpuDispatcher->release();
	//m_PxPhysics->release(); // this cleans up all physics objects
	//m_PxFoundation->release();
}
void PhysicsSystem::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;

	{
		// Create submeshes for colliders
		auto& resMan = ResourceManager::getInstance();
		// Box
		auto box = PrimitiveCreation::createCubeMesh();
		m_boxColliderMesh = resMan.storeSubmesh(std::move(box));
		
		// Sphere
		auto sphere = PrimitiveCreation::createSphere(15, 30);
		m_sphereColliderMesh = resMan.storeSubmesh(std::move(sphere));
	}

	// Foundation
	m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxDefaultAllocator, m_PxDefaultErrorCallback);
	if (!m_PxFoundation)
		throw("PxCreateFoundation failed");
	
	// Physx Visual Debugger
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_PxPvd = PxCreatePvd(*m_PxFoundation);
	m_PxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// Physics
	m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true, m_PxPvd);
	if (!m_PxPhysics) {
		throw("PxCreatePhysics failed");
	}

	// Cooking
	m_PxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_PxFoundation, PxCookingParams(PxTolerancesScale()));
	if (!m_PxCooking)
		throw("PxCreateCooking failed!");

	// Create PhysX scene
	PxSceneDesc sceneDesc(m_PxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_PxDefaultCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_PxDefaultCpuDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_PxScene = m_PxPhysics->createScene(sceneDesc);

	// Default material
	m_defaultMaterial = m_PxPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// Add a ground plane
	//PxRigidStatic* groundPlane = PxCreatePlane(*m_PxPhysics, PxPlane(0, 1, 0, 0), *m_defaultMaterial);
	//m_PxScene->addActor(*groundPlane);

	m_timeAccumulate = 0.0f;
	m_stepSize = 1.0f / 60.0f;

}

void PhysicsSystem::update(float deltaT)
{
	if (!m_physicsEnabled)
		return;
	m_timeAccumulate += deltaT;
	if (m_timeAccumulate < m_stepSize)
		return;

	m_timeAccumulate -= m_stepSize;

	m_PxScene->simulate(m_stepSize);
	m_PxScene->fetchResults(true);

	PxU32 nbActors = m_PxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		m_PxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		
		for (auto& ent : m_entities) {
			for (auto& act : actors) {
				if (act->userData == (void*)ent) {
					auto& t = m_ecs->getComponent<Transform>(ent);
					t.position.x = act->getGlobalPose().p.x;
					t.position.y = act->getGlobalPose().p.y;
					t.position.z = act->getGlobalPose().p.z;
					
					PxVec3 orientationEuler;
					//act->getGlobalPose().q.toRadiansAndUnitAxis(0.f, );
					t.orientation.x = act->getGlobalPose().q.x;
					t.orientation.y = act->getGlobalPose().q.y;
					t.orientation.z = act->getGlobalPose().q.z;
					t.orientation.w = act->getGlobalPose().q.w;

					InternalEvents::MarkNodeTransformStaleEvent::notify(ent);
				}
			}
		}
	}
}

void PhysicsSystem::drawColliders()
{
	if (!m_visibleColliders)
		return;
	PxU32 nbActors = m_PxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		m_PxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);

		for (auto& ent : m_entities) {
			for (auto& act : actors) {
				if (act->userData == (void*)ent) {
					auto& t = m_ecs->getComponent<Transform>(ent);
					auto& r = m_ecs->getComponent<RigidBody>(ent);

					// Draw collider
					auto& sdrMan = ShaderManager::getInstance();
					sdrMan.setUniformDrawSpecific("model", t.modelMatrix);
					sdrMan.forwardUniformsDrawSpecific();
					if (r.type == RigidBody::RigidBodyType::DBOX
						|| r.type == RigidBody::RigidBodyType::SBOX)
						ResourceManager::getInstance().getSubmesh(m_boxColliderMesh)->draw();
					else if (r.type == RigidBody::RigidBodyType::DSPHERE
						|| r.type == RigidBody::RigidBodyType::SSPHERE)
						ResourceManager::getInstance().getSubmesh(m_sphereColliderMesh)->draw();
				}
			}
		}
	}
}

void PhysicsSystem::onEntityAdded(Entity ent)
{
	auto& t = m_ecs->getComponent<Transform>(ent);
	auto& r = m_ecs->getComponent<RigidBody>(ent);

	// Add a physx shape
	PxTransform tm(t.worldPos.x, t.worldPos.y, t.worldPos.z);
	glm::quat q = glm::normalize(t.orientation);

	tm.q = PxQuat(q.x, q.y, q.z, q.w);
	PxShape* shape = nullptr;
	if (r.type == RigidBody::RigidBodyType::DBOX) {
		shape = m_PxPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *m_defaultMaterial);
		PxRigidDynamic* body = m_PxPhysics->createRigidDynamic(tm);
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		body->userData = (void*)ent; // attach entity id
		m_PxScene->addActor(*body);
	}
	else if (r.type == RigidBody::RigidBodyType::DSPHERE) {
		shape = m_PxPhysics->createShape(PxSphereGeometry(1.0f), *m_defaultMaterial);
		PxRigidDynamic* body = m_PxPhysics->createRigidDynamic(tm);
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		body->userData = (void*)ent; // attach entity id
		m_PxScene->addActor(*body);
	}
	else if (r.type == RigidBody::RigidBodyType::SBOX) {
		shape = m_PxPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *m_defaultMaterial);
		PxRigidStatic* body = m_PxPhysics->createRigidStatic(tm);
		body->attachShape(*shape);
		body->userData = (void*)ent; // attach entity id
		m_PxScene->addActor(*body);
	}
	else if (r.type == RigidBody::RigidBodyType::SSPHERE) {
		shape = m_PxPhysics->createShape(PxSphereGeometry(1.0f), *m_defaultMaterial);
		PxRigidStatic* body = m_PxPhysics->createRigidStatic(tm);
		body->attachShape(*shape);
		body->userData = (void*)ent; // attach entity id
		m_PxScene->addActor(*body);
	}
	else if (r.type == RigidBody::RigidBodyType::TRIANGLEMESH) {
		auto signature = m_ecs->getEntitySignature(ent);
		if (!signature.test(m_ecs->getComponentType<Model3D>()))
			return;
		// use one submesh from the mesh
		for (auto const& smesh : m_ecs->getComponent<Model3D>(ent).mesh->submeshlist) {
			PxTriangleMeshDesc meshDesc;
			meshDesc.points.count = smesh->vertices.size();
			meshDesc.points.stride = sizeof(Vertex);
			meshDesc.points.data = &smesh->vertices[0];

			meshDesc.triangles.count = smesh->indices.size() / 3;
			meshDesc.triangles.stride = 3 * sizeof(unsigned int);
			meshDesc.triangles.data = &smesh->indices[0];

			PxDefaultMemoryOutputStream writeBuffer;
			PxTriangleMeshCookingResult::Enum result;
			bool status = m_PxCooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
			if (!status)
				return;

			PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			PxTriangleMesh* aTriangleMesh = m_PxPhysics->createTriangleMesh(readBuffer);

			PxRigidStatic* body = m_PxPhysics->createRigidStatic(tm);
			PxTriangleMeshGeometry triGeom;
			triGeom.triangleMesh = aTriangleMesh;
			//shape = PxRigidActorExt::createExclusiveShape();
			shape = PxRigidActorExt::createExclusiveShape(*body, triGeom, *m_defaultMaterial);
			//body->attachShape(*shape);
			body->userData = (void*)ent; // attach entity id
			m_PxScene->addActor(*body);
		}
		

	}
	shape->release();
}

void PhysicsSystem::onEntityRemoved(Entity ent)
{
	PxU32 nbActors = m_PxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		m_PxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		for (auto& act : actors) {
			if (act->userData == (void*)ent) {
				m_PxScene->removeActor(*act);
			}
		}
	}
}

void PhysicsSystem::physicsOn()
{
	m_physicsEnabled = true;
}

void PhysicsSystem::physicsOff()
{
	m_physicsEnabled = false;
}

void PhysicsSystem::toggleColliderVisibility(bool f)
{
	m_visibleColliders = f;
}
