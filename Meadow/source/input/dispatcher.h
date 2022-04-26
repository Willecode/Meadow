//#pragma once
//#include <functional>
//#include "inputevent.h"
//
///*
//* Register input events and callback functions.
//*/
//class Dispatcher
//{
//public:
//	//void subscribe(const char* eventType, std::function<void(const char*)> handler);
//
//	template <typename HandlerType>
//	void subscribe(const char* eventType, HandlerType handler);
//
//	template <typename EventType>
//	void notify(EventType event);
//private:
//	std::unordered_map<const char*, std::vector<std::function<void(const char*)>>> m_handlers;
//	std::unordered_map<const char*, std::vector<std::function<void(const char*, float, float)>>> m_handlers2f;
//};
//
