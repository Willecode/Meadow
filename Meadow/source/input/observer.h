#pragma once
#include "input/inputevent.h"
class Observer
{
private:
	virtual void eventHandler(const char* eventType) = 0;
};

