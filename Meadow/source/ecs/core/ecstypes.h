#pragma once
#include <cstdint>
#include <bitset>

namespace ECSTypes {
	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 3000;
	const Entity NullEntity = MAX_ENTITIES;
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENT_TYPES = 32;
	using Signature = std::bitset<MAX_COMPONENT_TYPES>;
}
