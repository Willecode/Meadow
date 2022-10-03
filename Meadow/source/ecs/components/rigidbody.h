#pragma once

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
	RigidBody(): type(RigidBodyType::SBOX){}
};