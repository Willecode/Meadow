#include "rendererlocator.h"


void RendererLocator::initialize()
{
	m_renderer = nullptr;
}

Renderer* RendererLocator::getRenderer()
{
	return m_renderer;
}

void RendererLocator::provide(Renderer* renderer)
{
	m_renderer = renderer;
}




