#pragma once
#include <functional>
#include "inputevent.h"
/*
* Register input events and callback functions.
*/
class Dispatcher
{
public:
	void subscribe(const char* event, std::function<void(const char*)> handler);
	void subscribe2f(const char* event, std::function<void(const char*, float, float)> handler);
	void notify(const char* evnt);
	void notify2f(const char* evnt, float x, float y);
private:
	std::unordered_map<const char*, std::vector<std::function<void(const char*)>>> m_handlers;
	std::unordered_map<const char*, std::vector<std::function<void(const char*, float, float)>>> m_handlers2f;
};

