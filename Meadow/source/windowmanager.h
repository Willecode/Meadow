#pragma once
#include <memory>
#include <string>
#include <GLFW/glfw3.h>
/*
* Manages windows
*/
class WindowManager
{
	typedef void (*GLProc)(void);
public:
	static float width;
	static float height;
public:
	static WindowManager& getInstance();
	WindowManager(WindowManager const&) = delete;
	void operator=(WindowManager const&) = delete;
	~WindowManager();
	bool createWindow(std::string title);
	bool shouldClose();
	void swapBuffers();
	GLProc getProcAddress();
	GLFWwindow* getWindow();
	float getTime();
	void pollEvents();
private:
	WindowManager();
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
	void closeWindowEventHandler();
	void MouseLockHandler();
	void MouseUnlockHandler();
};

