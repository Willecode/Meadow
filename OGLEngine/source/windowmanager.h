#pragma once
#include <GLFW/glfw3.h>
#include <memory>

/*
* Manages one window
*/
class WindowManager
{
public:
	WindowManager();
	bool createWindow();
	bool shouldClose();
	void swapBuffers();

	static float width;
	static float height;

private:
	GLFWwindow* m_window;
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

