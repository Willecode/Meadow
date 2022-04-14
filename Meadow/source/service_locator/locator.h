#pragma once
#include "../renderer/renderer.h"
#include "logger.h"
//#include "../windowmanager.h"

class Locator
{
public:
	static void initialize();
	//static WindowManager* getWindowMan();
	static Renderer* getRenderer();
	static Logger* getLogger();
	static void provide(Renderer* renderer);
	//static void provide(WindowManager* wind);
	static void provide(Logger* logger);

private:
	inline static Renderer* m_renderer;
	//inline static WindowManager* m_windowMan;
	inline static Logger* m_logger;
};

