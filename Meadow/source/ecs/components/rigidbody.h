#pragma once
#include <functional>
#include "transform.h"
struct RigidBody {
	enum class RigidBodyType {
		DBOX = 0,
		SBOX,
		DCAPSULE,
		SCAPSULE,
		TRIANGLEMESH,
		DSPHERE,
		SSPHERE
	};
	RigidBodyType type;
	std::function<void(Transform)> onTouch;
	bool trackTouches;
	RigidBody() : type(RigidBodyType::SBOX), trackTouches(false), onTouch([](auto t) {}) {}
};