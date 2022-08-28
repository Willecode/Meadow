#include "importutility.h"
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <ShlObj.h>

//std::string OpenFile(const char* filter, GLFWwindow* window)
//{
//	//Create a new OPENFILENAMEA struct
//	OPENFILENAMEA ofn;
//
//	// Zero out the ofn's memory block
//	ZeroMemory(&ofn, sizeof(OPENFILENAME));
//
//	// Don't know why this is done, seems dumb but has to be done
//	ofn.lStructSize = sizeof(OPENFILENAME);
//
//	// Pass the handle to the window that will own this dialog
//	ofn.hwndOwner = glfwGetWin32Window(window);
//
//	// Buffer that will contain info about the opened file
//	CHAR szFile[260] = { 0 };
//	ofn.lpstrFile = szFile;
//	ofn.nMaxFile = sizeof(szFile);
//
//	// Try to find a good starting directory for the dialog
//	CHAR currentDir[256] = { 0 };
//	if (GetCurrentDirectoryA(256, currentDir)) {}
//	ofn.lpstrInitialDir = currentDir;
//
//	// filter visible files in dialog (for example, ".TXT;.DOC;.BAK")
//	ofn.lpstrFilter = filter;
//	ofn.nFilterIndex = 1;
//
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST /* | OFN_NOCHANGEDIR*/;
//
//	if (GetOpenFileNameA(&ofn) == TRUE)
//		return ofn.lpstrFile;
//
//	return "Error: Failed to open file\n";
//
//
//}
//
std::string ImportUtility::openFileBrowser()
{

	return "yippee";
}
