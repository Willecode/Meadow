#pragma once
//#include <spdlog/spdlog.h>
/*
* Logger class.
* TODO: implement using templates. also, this be broken. make proper one. I can't be arsed rn.
* spdlog caused WINAPI redefinition a billion times.
*/

/*
* This can be used instead of spdlogger to disable logging easily
*/
class dumb {
public:
	void info(const char*, const char* = "", const char* = "") {};
	void info(const char*, float d, float) {};
	void error(const char*, int, const char* = "") {};
};
class Logger
{
public:
	void init();
	dumb* getLogger();
private:
	//std::shared_ptr<spdlog::logger> m_spdLogger;
	dumb m_dumb;
};

