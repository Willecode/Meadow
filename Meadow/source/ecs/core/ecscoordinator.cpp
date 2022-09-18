#include "ecscoordinator.h"
#include "ecs/components/transform.h"
void ECSCoordinator::init()
{
	m_componentManager = std::make_unique<ComponentManager>();
	m_scene = std::make_unique<Scene>();
	m_systemManager = std::make_unique<SystemManager>();
	
	registerComponent<Transform>(); // All entities have transform
}

Entity ECSCoordinator::createEntity()
{
	auto ent = m_scene->createEntity();
	Transform t;
	addComponent(ent, t);
	return ent;
}

void ECSCoordinator::destroyEntity(Entity entity)
{
	m_scene->destroyEntity(entity);
	m_componentManager->entityDestroyed(entity);
	m_systemManager->entityDestroyed(entity);
}
