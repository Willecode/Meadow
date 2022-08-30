#pragma once
#include <string>
#include <map>
#include "windowmanager.h"
class ImportUtility
{
public:
	enum class FileType {
		PNGJPG = 0,
		OBJFBX
	};
	static bool fileBrowserOpenFile(std::string& retFilename, FileType t);
private:
	static const std::map<FileType, const char*> m_filetypes;
	static bool OpenFile(std::string& retFilename, const char* filter, GLFWwindow* window);
};