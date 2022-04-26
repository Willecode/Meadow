#pragma once
#include <vector>
#include <functional>
namespace InputEvents
{
	class CameraUpEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class CameraDownEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class CameraForwardEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class CameraBackwardEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class CameraLeftEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class CameraRightEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class MouseMoveEvent
	{
	public:
		static void subscribe(std::function<void(float, float)> f) {
			m_handlers.push_back(f);
		}
		static void notify(float mouseX, float mouseY) {
			for (auto h : m_handlers) {
				h(mouseX, mouseY);
			}
		}
	private:
		inline static std::vector<std::function<void(float, float)>> m_handlers;
	};

	class CloseWindowEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class MouseButtonLeftPressedEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class MouseButtonLeftReleasedEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class MouseLockEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

	class MouseUnlockEvent
	{
	public:
		static void subscribe(std::function<void()> f) {
			m_handlers.push_back(f);
		}
		static void notify() {
			for (auto h : m_handlers) {
				h();
			}
		}
	private:
		inline static std::vector<std::function<void()>> m_handlers;
	};

}