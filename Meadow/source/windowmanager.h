#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include "renderer/renderer.h"
#include <string>

/*
* Manages one window
*/
class WindowManager
{
	typedef void (*GLProc)(void);

public:
	WindowManager();
	~WindowManager();
	bool createWindow(std::string title);
	bool shouldClose();
	void swapBuffers();
	GLProc getProcAddress();

	static float width;
	static float height;

private:
	GLFWwindow* m_window;
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

