#pragma once
#include <physx/PxPhysicsAPI.h>
#include "service_locator/loggerlocator.h"
#include "ecs/core/ecscoordinator.h"
#include "ecs/components/rigidbody.h"
using namespace physx;
class CollisionCallback : public PxSimulationEventCallback
{
public:
	CollisionCallback(ECSCoordinator* ecs): m_ecs(ecs) {

	}
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
	{
		LoggerLocator::getLogger()->getLogger()->info("COLLISON TOUCH");
		Entity ent1 = (Entity)(pairHeader.actors[0]->userData);
		Entity ent2 = (Entity)(pairHeader.actors[1]->userData);
		auto& rb1 = m_ecs->getComponent<RigidBody>(ent1);
		auto& t1  = m_ecs->getComponent<Transform>(ent1);
		auto& rb2 = m_ecs->getComponent<RigidBody>(ent2);
		auto& t2  = m_ecs->getComponent<Transform>(ent2);

		rb1.onTouch(t1);
		rb1.onTouch(t2);
	}
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {};
	void onWake(PxActor** actors, PxU32 count) override {};
	void onSleep(PxActor** actors, PxU32 count) override {};
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override {};
	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {};
private:
	ECSCoordinator* m_ecs;
};