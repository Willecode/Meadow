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
#include "resource_management/importer.h"
/*
* Debug includes
* ******************/
#include "primitivecreation.h"
#include "scene/camera.h"
#include "assets/texture.h"
//*******************

#include "ui/ui.h"
#include "service_locator/rendererlocator.h"
#include "service_locator/loggerlocator.h"
#include "windowmanager.h"
#include "ui/uidatascraper.h"
#include "postprocessing.h"
#include "lighting.h"
class Application
{
public:
	bool appFailed;

	Application();
	void run();
private:
	std::unique_ptr<Scene> m_scene;
	InputGather m_inputGather;
	UIDataScraper m_UIScraper;
	OpenGLRenderer m_renderer;
	ShaderManager m_shaderManager;
	Logger m_logger;
	UI m_ui;
	PostProcessing m_postProcessing;
	Lighting m_lighting;
	Importer m_importer;
};

