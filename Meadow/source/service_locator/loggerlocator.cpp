#include "loggerlocator.h"

void LoggerLocator::initialize()
{
		m_logger = nullptr;
}

void LoggerLocator::provide(Logger* logger)
{
	m_logger = logger;
}

Logger* LoggerLocator::getLogger()
{
	return m_logger;
}