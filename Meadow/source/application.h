#pragma once
#include "resource_management/imageloader.h"
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
	std::unique_ptr<Scene> m_scene;
	WindowManager m_windowManager;
	OpenGLRenderer m_renderer;
	Logger m_logger;

	/*
	* Debug thingies, delete these when no longer used
	*/
	std::shared_ptr<Mesh> m;
	unsigned int sdrId;
};

