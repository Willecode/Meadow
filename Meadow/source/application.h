#pragma once
#include "resource_management/imageloader.h"
#include "scene/scene.h"
//#include "logger.h"
#include "windowmanager.h"
#include "input/inputgather.h"
#include "input/dispatcher.h"
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
	InputGather m_inputGather;
	Dispatcher m_dispatcher;
	OpenGLRenderer m_renderer;
	ShaderManager m_shaderManager;
	Logger m_logger;

};

