#pragma once
#include "inputevent.h"
#include <functional>
#include "input/observer.h"
/*
* Register input events and callback functions.
*/
class Dispatcher
{
public:
	void subscribe(const char* event, std::function<void(const char*)> handler);
	void notify(const char* evnt);
private:
	std::unordered_map<const char*, std::vector<std::function<void(const char*)>>> m_handlers;
};

