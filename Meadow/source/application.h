#pragma once
#include "resource_management/imageloader.h"
#include "scene.h"
//#include "logger.h"
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
	ShaderManager m_shaderManager;
	Logger m_logger;

};

