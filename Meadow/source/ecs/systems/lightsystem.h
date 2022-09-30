#pragma once
#include "ecs/core/system.h"
#include <ecs/core/ecscoordinator.h>
class LightSystem : public System
{
public:
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
private:
	ECSCoordinator* m_ecs;
	// parse str and if "[]" found then change to "[<id>]
	std::string injectId(const std::string& str, int id);
};

