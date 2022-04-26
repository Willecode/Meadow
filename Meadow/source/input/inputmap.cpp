#include "inputmap.h"

std::function<void()> InputMap::getEventFunc(int key, bool pressed)
{
	auto it = m_map.find(std::make_pair(key, pressed));
	if (it != m_map.end()) {
		return it->second;
	}
	/*
	* return null function
	*/
	return std::bind(&InputMap::nullFunc, this);
}
