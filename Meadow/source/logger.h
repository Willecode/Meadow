#pragma once
#include <spdlog/spdlog.h>
/*
* Logger class.
* TODO: implement using templates. also, this be broken. make proper one. I can't be arsed rn
*/
class Logger
{
public:
	void init();
	spdlog::logger* getLogger() { return m_spdLogger.get(); };
private:
	std::shared_ptr<spdlog::logger> m_spdLogger;
};

