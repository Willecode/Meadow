#pragma once
#include <memory>
#include <string>
#include "input/dispatcher.h"
#include <GLFW/glfw3.h>
/*
* Manages one window
*/
class WindowManager
{
	typedef void (*GLProc)(void);
public:
	static float width;
	static float height;
public:
	WindowManager();
	~WindowManager();
	bool createWindow(std::string title, Dispatcher* disp);
	bool shouldClose();
	void swapBuffers();
	GLProc getProcAddress();
	GLFWwindow* getWindow();
	float getTime();
	void pollEvents();
private:
	GLFWwindow* m_window;

private:
	/*
	* glfw callbacks
	*/
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void errorCallback(int error, const char* description);
	/*
	* Meadow input event callbacks
	*/
	void closeWindowEventHandler(const char* eventType);
	void MouseLockHandler(const char* eventType);
};

