#include "logger.h"
//#include "spdlog/sinks/stdout_color_sinks.h"
void Logger::init()
{
	//spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
	//m_spdLogger = spdlog::stdout_color_mt("MAIN LOGGER");
	//m_spdLogger->set_level(spdlog::level::trace);
}

dumb* Logger::getLogger()
{
	return &m_dumb;
}
