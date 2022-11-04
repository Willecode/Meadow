#pragma once
#include "ecs/core/system.h"
#include <ecs/core/ecscoordinator.h>
#include <fmod/fmod.hpp>
#include <map>

class AudioSystem : public System
{
public:
	AudioSystem();
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
	void playSound(unsigned int s);
	bool createSound(unsigned int s, std::string path);
	
	~AudioSystem();
private:
	ECSCoordinator* m_ecs;
	FMOD::System* m_system;

	std::map<unsigned int, FMOD::Sound*> m_sounds;
};

