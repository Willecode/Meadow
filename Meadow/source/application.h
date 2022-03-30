#pragma once
#include "ImageCache.h"
#include "scene.h"
#include "windowmanager.h"
#include "renderer/openglrenderer.h"

#include <memory>

class Application
{
public:
	bool appFailed;

	Application();
	void run();
private:
	ImageCache m_textureCache;
	std::unique_ptr<Scene> m_scene;
	WindowManager m_windowManager;
	OpenGLRenderer m_renderer;
};

