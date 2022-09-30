#include "systemmanager.h"

void SystemManager::entityDestroyed(Entity entity)
{
	for (auto const& pair : m_systems)
	{
		auto const& system = pair.second;

		system->removeEntity(entity);
	}
}

void SystemManager::entitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (auto const& pair : m_systems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = m_signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->addEntity(entity);
		}
		else
		{
			system->removeEntity(entity);
		}
	}
}
