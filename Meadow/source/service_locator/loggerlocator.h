#pragma once
#include "logger.h"
class LoggerLocator
{
public:
	static void initialize();
	static Logger* getLogger();
	static void provide(Logger* logger);
private:
	inline static Logger* m_logger;
};

