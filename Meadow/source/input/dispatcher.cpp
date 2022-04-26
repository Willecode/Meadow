#include "dispatcher.h"
#include "service_locator/locator.h"

//void Dispatcher::subscribe(const char* event, std::function<void(const char*)> handler)
//{
//	m_handlers[event].push_back(handler);
//}
//
//void Dispatcher::subscribe2f(const char* event, std::function<void(const char*, float, float)> handler)
//{
//	m_handlers2f[event].push_back(handler);
//}
//
//void Dispatcher::notify(const char* evnt)
//{
//	//Locator::getLogger()->getLogger()->info("Notified event: {}", evnt);
//	auto it = m_handlers.find(evnt);
//	if (it != m_handlers.end()) {
//		for (auto h : it->second)
//			h(evnt);
//	}
//}
//
//void Dispatcher::notify2f(const char* evnt, float x, float y)
//{
//	auto it = m_handlers2f.find(evnt);
//	if (it != m_handlers2f.end()) {
//		for (auto h : it->second)
//			h(evnt, x, y);
//	}
//}

//template<typename HandlerType>
//inline void Dispatcher::subscribe(const char* eventType, HandlerType handler)
//{
//	HandlerMap<HandlerType>::getMap()[eventType].push_back(handler);
//}
//
//template<typename EventType>
//void Dispatcher::notify(EventType event)
//{
//	for (auto h : HandlerMap<EventType>::getMap()) {
//		Locator::getLogger()->getLogger()->info(event.getType());
//	}
//}
