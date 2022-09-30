/*
* The *Event classes in this file each have:
* - a list of handlers
* - a subscribe function which can be used to register handlers to the event class
* - a notify function which calls all of the registered handlers
* Why?:
* - this way the events may specify the type of the handler funcs they allow to subscribe,
*	which is required since the event notifications may carry data that the handler should
*	take as arguments.
* 
*/
#pragma once
#include <vector>
#include <functional>
#include "assets/texture.h"
#include "shader/shadermanager.h"
namespace InputEvents
{
	/*
	* This class stores the callbacks of notified events.
	*/
	class EventQueue {
	public:
		static void addCallback(std::function<void(unsigned int)> f, unsigned int arg) {
			m_voidUintHandlers.push_back(std::make_pair(f, arg));
		}
		static void processQueue() {
			for (auto h : m_voidUintHandlers) {
				h.first(h.second);
			}
		}
	private:
		inline static std::vector<std::pair<std::function<void(unsigned int)>, unsigned int>> m_voidUintHandlers;
	};
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

	class WindowBordersToggleEvent
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

	class WindowDimensionsChangedEvent
	{
	public:
		static void subscribe(std::function<void(int, int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(int width, int height) {
			for (auto h : m_handlers) {
				h(width, height);
			}
		}
	private:
		inline static std::vector<std::function<void(int, int)>> m_handlers;
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

	class AddNodeEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int x) {
			for (auto h : m_handlers) {
				h(x);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};
	class DuplicateNodeEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int x) {
			for (auto h : m_handlers) {
				h(x);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};

	class DeleteMaterialEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int x) {
			for (auto h : m_handlers) {
				EventQueue::addCallback(h, x);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};

	class SetNodeMeshEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int, unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int nodeid, unsigned int meshid) {
			for (auto h : m_handlers) {
				h(nodeid, meshid);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int, unsigned int)>> m_handlers;
	};

	class SetSubmeshMaterialEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int, unsigned int, unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int meshid, unsigned int submeshid, unsigned int materialid) {
			for (auto h : m_handlers) {
				h(meshid, submeshid, materialid);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int, unsigned int, unsigned int)>> m_handlers;
	};

	class setMaterialTextureEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int, unsigned int, Texture::TextureType)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int materialid, unsigned int textureid, Texture::TextureType textureType) {
			for (auto h : m_handlers) {
				h(materialid, textureid, textureType);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int, unsigned int, Texture::TextureType)>> m_handlers;
	};

	class importTextureREvent
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

	class importTextureRGBEvent
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

	class importTextureSRGBEvent
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

	class import3DModelEvent
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

	class PostprocSharpnessEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};
	class PostprocGrayscaleEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};
	class PostprocNegativeEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};

	class LightingBlinnEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};
	class MSAAToggleEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};
	class ShaderHotReloadEvent
	{
	public:
		static void subscribe(std::function<void(ShaderManager::ShaderType)> f) {
			m_handlers.push_back(f);
		}
		static void notify(ShaderManager::ShaderType s) {
			for (auto h : m_handlers) {
				h(s);
			}
		}
	private:
		inline static std::vector<std::function<void(ShaderManager::ShaderType)>> m_handlers;
	};

	class MainMenuVisibilityToggleEvent
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

	class AssetWindowVisibilityEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};

	class SceneGraphVisibilityEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};

	class SceneNodeLightsourceAddEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int id) {
			for (auto h : m_handlers) {
				h(id);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};

	class SceneNodeLightsourceRemoveEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int id) {
			for (auto h : m_handlers) {
				h(id);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};

	class SceneNodeInspectorVisibilityEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int id) {
			for (auto h : m_handlers) {
				h(id);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};

	class SetActiveNodeEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int id) {
			for (auto h : m_handlers) {
				h(id);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int)>> m_handlers;
	};

	class SetNodeSelectionEvent
	{
	public:
		static void subscribe(std::function<void(unsigned int, bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(unsigned int id, bool f) {
			for (auto h : m_handlers) {
				h(id, f);
			}
		}
	private:
		inline static std::vector<std::function<void(unsigned int, bool)>> m_handlers;
	};

	class PhysicsToggleEvent
	{
	public:
		static void subscribe(std::function<void(bool)> f) {
			m_handlers.push_back(f);
		}
		static void notify(bool flag) {
			for (auto h : m_handlers) {
				h(flag);
			}
		}
	private:
		inline static std::vector<std::function<void(bool)>> m_handlers;
	};

}