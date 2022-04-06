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

Logger* Locator::getLogger()
{
	return m_logger;
}

void Locator::provide(Renderer* renderer)
{
	m_renderer = renderer;
}

void Locator::provide(WindowManager* wind)
{
	m_windowMan = wind;
}

void Locator::provide(Logger* logger)
{
	m_logger = logger;
}


