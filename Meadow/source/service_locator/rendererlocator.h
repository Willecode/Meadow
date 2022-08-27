#pragma once
#include "../renderer/renderer.h"

class RendererLocator
{
public:
	static void initialize();
	static Renderer* getRenderer();
	static void provide(Renderer* renderer);

private:
	inline static Renderer* m_renderer;
};

