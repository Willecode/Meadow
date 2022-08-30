#include "importutility.h"


#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <windows.h>

const std::map<ImportUtility::FileType, const char*> ImportUtility::m_filetypes = {
	{ImportUtility::FileType::PNGJPG, "PNG,JPG\0*.png\0\0"},
	{ImportUtility::FileType::OBJ, "OBJ\0*.obj\0\0"}
};

bool ImportUtility::OpenFile(std::string& retFilename, const char* filter, GLFWwindow* window)
{
	//Create a new OPENFILENAMEA struct
	OPENFILENAMEA ofn;

	// Zero out the ofn's memory block
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	// Don't know why this is done, seems dumb but has to be done
	ofn.lStructSize = sizeof(OPENFILENAME);

	// Pass the handle to the window that will own this dialog
	ofn.hwndOwner = glfwGetWin32Window(window);

	// Buffer that will contain info about the opened file
	CHAR szFile[260] = { 0 };
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	// Try to find a good starting directory for the dialog
	CHAR currentDir[256] = { 0 };
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;

	// filter visible files in dialog (for example, ".TXT;.DOC;.BAK")
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST /* | OFN_NOCHANGEDIR*/;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		retFilename = ofn.lpstrFile;
		return true;

	}

	return false;


}

bool ImportUtility::fileBrowserOpenFile(std::string& retFilename,FileType t)
{
	return OpenFile(retFilename, m_filetypes.at(t), WindowManager::getInstance().getWindow());
}
