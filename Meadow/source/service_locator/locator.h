#pragma once
#include "../windowmanager.h"
#include "../renderer/renderer.h"

class Locator
{
public:
	static void initialize();
	static WindowManager* getWindowMan();
	static Renderer* getRenderer();
	static void provide(Renderer* renderer);
	static void provide(WindowManager* wind);

private:
	inline static Renderer* m_renderer;
	inline static WindowManager* m_windowMan;

};

