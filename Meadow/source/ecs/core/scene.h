#pragma once
#include "ecstypes.h"
#include "scenegraph.h"
#include <queue>
#include <array>
using namespace ECSTypes;

class Scene
{
public:
	Scene();
	Entity createEntity();
	void destroyEntity(Entity ent);
	void setSignature(Entity ent, Signature signature);
	Signature getSignature(Entity ent);

private:
	std::queue<Entity> m_availableEntities{};
	std::array<Signature, MAX_ENTITIES> m_signatures{};
	uint32_t m_livingEntityCount{};

};

