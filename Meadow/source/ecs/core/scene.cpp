#include "scene.h"
#include "ecs/core/internalevents.h"
#include <cassert>
Scene::Scene()
{
	for (Entity ent = 0; ent < MAX_ENTITIES; ++ent)
	{
		m_availableEntities.push(ent);
	}
	// Create root node
	createEntity();
}
Entity Scene::createEntity()
{
	assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

	Entity id = m_availableEntities.front();
	m_availableEntities.pop();
	m_livingEntityCount++;

	InternalEvents::EntityCreatedEvent::notify(id);

	return id;
}
void Scene::destroyEntity(Entity ent)
{
	assert(ent < MAX_ENTITIES && "Entity out of range.");

	m_signatures[ent].reset();
	m_availableEntities.push(ent);
	m_livingEntityCount--;

	InternalEvents::EntityDeletedEvent::notify(ent);
}
void Scene::setSignature(Entity ent, Signature signature)
{
	assert(ent < MAX_ENTITIES && "Entity out of range.");

	m_signatures[ent] = signature;
}
Signature Scene::getSignature(Entity ent)
{
	assert(ent < MAX_ENTITIES && "Entity out of range.");

	return m_signatures[ent];
}

