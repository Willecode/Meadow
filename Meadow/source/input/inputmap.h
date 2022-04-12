#pragma once
#include "inputevent.h"
#include <map>
#include <memory>
/*
* Recieve glfw key input and convert it into input events.
* Read key input mapping from a file (in the future).
*/
class InputMap
{
public:
	InputMap() = default;
	InputMap(std::map<int, std::unique_ptr<InputEvent>> map):
		m_map(std::move(map))
	{

	}
	InputEvent* getEvent(int key) {
		auto it = m_map.find(key);
		if (it != m_map.end()) {
			return it->second.get();
		}
		return nullptr;
	}

private:
	std::map<int, std::unique_ptr<InputEvent>> m_map;
};

