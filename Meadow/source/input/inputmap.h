#pragma once
#include <map>
#include <memory>
#include <functional>
#include "inputevents.h"
/*
* Recieve glfw key input and convert it into input events.
* Read key input mapping from a file (in the future).
*/
class InputMap
{
public:
	InputMap() = default;
	InputMap(std::map<std::pair<int, bool>, std::function<void()>> map):
		m_map(std::move(map))
	{
	}
	std::function<void()> getEventFunc(int key, bool pressed);

private:
	void nullFunc(){}
	/*
	* Map glfw key id and pressed status (true/false) to a Meadow input event 
	*/
	std::map<std::pair<int, bool>, std::function<void()>> m_map;
};

