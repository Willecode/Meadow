#pragma once
#include "ecs/core/system.h"
#include "ecs/core/ecscoordinator.h"
class RenderSystem : public System
{
public:
	void init();
	void update(float deltaT, ECSCoordinator& ecs);
};

