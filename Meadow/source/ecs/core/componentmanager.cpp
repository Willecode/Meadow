#include "componentmanager.h"
#include "ecs/components/transform.h"
#include "ecs/core/internalevents.h"


void ComponentManager::entityDestroyed(Entity entity)
{
	for (auto const& pair : m_componentArrays)
	{
		auto const& component = pair.second;

		component->entityDestroyed(entity);
	}
}
