#pragma once
#include "ecs/core/system.h"
#include <ecs/core/ecscoordinator.h>
#include <fmod/fmod.hpp>

class AudioSystem : public System
{
public:
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
private:
	ECSCoordinator* m_ecs;
	FMOD::System* m_system;
	FMOD::Sound* m_sound;
};

