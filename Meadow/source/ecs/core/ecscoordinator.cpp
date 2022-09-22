#include "ecscoordinator.h"
#include "ecs/components/transform.h"
void ECSCoordinator::init()
{
	m_componentManager = std::make_unique<ComponentManager>();
	m_entityManager = std::make_unique<EntityManager>();
	m_systemManager = std::make_unique<SystemManager>();
	
	registerComponent<Transform>(); // All entities have transform
}

Entity ECSCoordinator::createEntity()
{
	auto ent = m_entityManager->createEntity();
	Transform t;
	addComponent(ent, t);
	return ent;
}

void ECSCoordinator::destroyEntity(Entity entity)
{
	m_entityManager->destroyEntity(entity);
	m_componentManager->entityDestroyed(entity);
	m_systemManager->entityDestroyed(entity);
}

Signature ECSCoordinator::getEntitySignature(Entity ent) const
{
	return m_entityManager->getSignature(ent);
}
