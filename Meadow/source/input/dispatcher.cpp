#include "dispatcher.h"
#include "service_locator/locator.h"

void Dispatcher::subscribe(const char* event, std::function<void(const char*)> handler)
{
	m_handlers[event].push_back(handler);
}

void Dispatcher::notify(const char* evnt)
{
	//Locator::getLogger()->getLogger()->info("Notified event: {}", evnt);
	auto it = m_handlers.find(evnt);
	if (it != m_handlers.end()) {
		for (auto h : it->second)
			h(evnt);
	}
}
