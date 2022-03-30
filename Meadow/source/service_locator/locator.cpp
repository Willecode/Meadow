#include "locator.h"


void Locator::initialize()
{
	m_renderer = nullptr;
	m_renderer = nullptr;
}

WindowManager* Locator::getWindowMan()
{
	return m_windowMan;
}

Renderer* Locator::getRenderer()
{
	return m_renderer;
}

void Locator::provide(Renderer* renderer)
{
	m_renderer = renderer;
}

void Locator::provide(WindowManager* wind)
{
	m_windowMan = wind;
}


