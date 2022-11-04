#pragma once
#include "input/inputevents.h"
#include "ecs/components/transform.h"
/*
* Funcs for rigidbody collisions
*/
namespace TouchFuncs {
	void playSound(unsigned int s, const Transform& t) {
		InputEvents::PlaySoundEvent::notify(s);
	}
}