#pragma once
#include "ecs/core/ecstypes.h"
#include <vector>
#include <functional>
using namespace ECSTypes;
namespace InternalEvents
{
	//class InternalEventQueue {
	//public:
	//	static void addCallback(std::function<void(Entity)> f, Entity arg) {
	//		m_voidEntityHandlers.push_back(std::make_pair(f, arg));
	//	}
	//	static void processQueue() {
	//		for (auto h : m_voidEntityHandlers) {
	//			h.first(h.second);
	//		}
	//		m_voidUintHandlers.clear();
	// 
	//	}
	//private:
	//	inline static std::vector<std::pair<std::function<void(Entity)>, Entity>> m_voidEntityHandlers;
	//};

	class EntityDeletedEvent
	{
	public:
		static void subscribe(std::function<void(Entity)> f) {
			m_handlers.push_back(f);
		}
		static void notify(Entity id) {
			for (auto h : m_handlers) {
				h(id);
			}
		}
	private:
		inline static std::vector<std::function<void(Entity)>> m_handlers;
	};

	class EntityCreatedEvent
	{
	public:
		static void subscribe(std::function<void(Entity)> f) {
			m_handlers.push_back(f);
		}
		static void notify(Entity id) {
			for (auto h : m_handlers) {
				h(id);
			}
		}
	private:
		inline static std::vector<std::function<void(Entity)>> m_handlers;
	};

}