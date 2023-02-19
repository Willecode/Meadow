#pragma once
#include <memory>
#include "logger.h"
#include "resource_management/imageloader.h"
#include "ecs/core/ecscoordinator.h"
#include "input/inputgather.h"

#include "renderer/openglrenderer.h"
#include "resource_management/imageloader.h"

#include "shader/shadermanager.h"
#include "resource_management/resourcemanager.h"
#include "resource_management/importer.h"
/*
* Debug includes
* ******************/
#include "utils/primitivecreation.h"
#include "assets/texture.h"
//*******************

#include "ui/ui.h"
#include "service_locator/rendererlocator.h"
#include "service_locator/loggerlocator.h"
#include "windowmanager.h"
#include "ui/uidatascraper.h"
#include "postprocessing/postprocessing.h"

// ECS
#include "ecs/systems/rendersystem.h"
#include "ecs/systems/camerasystem/camerasystem.h"
#include "ecs/systems/scenegraphsystem/scenegraphsystem.h"
#include "ecs/systems/lightsystem.h"
#include "ecs/systems/physicssystem.h"
#include "ecs/systems/audiosystem.h"
#include "ecs/systems/shadowmapsystem.h"
#include "ecs/core/componenteventhandler.h"

// Benchmarking
#include "benchmarking/benchmarksystem.h"

class Application
{
public:
	bool appFailed;

	Application();
	void run();
	void runGPUBenchmarked();
private:
	void initSystems();
	void registerComponents();
	void createDefaultScene();
	
	/*
	* Benchmarking
	*/
	void doBenchmark();
	bool mBenchmarkActive;

	//std::unique_ptr<Scene> m_scene;
	ECSCoordinator m_ecs;
	InputGather m_inputGather;
	UIDataScraper m_UIScraper;
	OpenGLRenderer m_renderer;
	Logger m_logger;
	UI m_ui;
	PostProcessing m_postProcessing;
	ComponentEventHandler m_componentEventHandler;
	Importer m_importer;
	std::shared_ptr<CameraSystem> m_cameraSystem;
	std::shared_ptr<RenderSystem> m_renderSystem;
	std::shared_ptr<SceneGraphSystem> m_sceneGraphSystem;
	std::shared_ptr<LightSystem> m_lightSystem;
	std::shared_ptr<PhysicsSystem> m_physicsSystem;
	std::shared_ptr<BenchmarkSystem> m_benchmarkSystem;
	std::shared_ptr<AudioSystem> m_audioSystem;
	std::shared_ptr<ShadowMapSystem> m_shadowMapSystem;
};

