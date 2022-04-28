#pragma once
#include <memory>
#include "logger.h"
#include "resource_management/imageloader.h"
#include "scene/scene.h"
#include "input/inputgather.h"

#include "renderer/openglrenderer.h"
#include "resource_management/imageloader.h"

#include "resource_management/shadermanager.h"
#include "resource_management/resourcemanager.h"

/*
* Debug includes
* ******************/
#include "primitivecreation.h"
#include "scene/camera.h"
#include "assets/texture.h"
//*******************

#include "ui/ui.h"
#include "service_locator/locator.h"
#include "windowmanager.h"
#include "ui/uidatascraper.h"
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
	UIDataScraper m_UIScraper;
	OpenGLRenderer m_renderer;
	ShaderManager m_shaderManager;
	Logger m_logger;
	UI m_ui;
};

