#pragma once
#include <map>
#include <memory>
#include "inputevent.h"
/*
* Recieve glfw key input and convert it into input events.
* Read key input mapping from a file (in the future).
*/
class InputMap
{
public:
	InputMap() = default;
	InputMap(std::map<std::pair<int, bool>, std::unique_ptr<InputEvent>> map):
		m_map(std::move(map))
	{

	}
	InputEvent* getEvent(int key, bool pressed) {
		auto it = m_map.find(std::make_pair(key, pressed));
		if (it != m_map.end()) {
			return it->second.get();
		}
		return nullptr;
	}

private:
	/*
	* Map glfw key id and pressed status (true/false) to a Meadow input event 
	*/
	std::map<std::pair<int, bool>, std::unique_ptr<InputEvent>> m_map;
};

