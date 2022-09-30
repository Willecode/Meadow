#pragma once
#include "ecs/core/system.h"
#include "ecs/core/ecscoordinator.h"
class RenderSystem : public System
{
public:
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
private:
	ECSCoordinator* m_ecs;
};

