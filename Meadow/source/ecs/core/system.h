#pragma once
#include <set>
#include "ecstypes.h"
using namespace ECSTypes;
class System
{
public:
	void addEntity(Entity ent) {
		m_entities.insert(ent);
		onEntityAdded(ent);
	};
	void removeEntity(Entity ent) {
		m_entities.erase(ent);
		onEntityRemoved(ent);
	};
protected:
	virtual void onEntityAdded(Entity ent) {};
	virtual void onEntityRemoved(Entity ent) {};
	std::set<Entity> m_entities;
};

