#pragma once
#include "ecstypes.h"
#include <queue>
#include <array>
using namespace ECSTypes;

class EntityManager
{
public:
	EntityManager();
	Entity createEntity(std::string name = "Entity");
	void destroyEntity(Entity ent);
	void setSignature(Entity ent, Signature signature);
	Signature getSignature(Entity ent);

	std::array<std::string, MAX_ENTITIES> entityNames;
private:
	std::queue<Entity> m_availableEntities{};
	std::array<Signature, MAX_ENTITIES> m_signatures{};
	uint32_t m_livingEntityCount{};

};

