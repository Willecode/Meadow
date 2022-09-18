#pragma once

#include "inputmap.h"

/*
* Listens to glfw input events and passes them to InputMaps
*/
class InputGather
{
public:
	enum class InputFlag {
		CameraUp = 0,
		CameraDown,
		CameraForward,
		CameraBackward,
		CameraLeft,
		CameraRight

	};
	float mouseX;
	float mouseY;
	float lastFrameMouseX;
	float lastFrameMouseY;

public:
	InputGather();
	void init(WindowManager* windowMan);
	void pollInputs();
	bool getInputFlag(InputFlag flag) const;
private:
	//These need to be static for them to be accessed in glfw callback functions, I think
	//--------------------------------------
	
	static inline bool initialized;
	/*
	* Flags that clients may check via pollInput()
	*/
	static inline std::map<InputFlag, bool> m_inputFlags;
	/*
	* Maps glfw keypresses to Meadow InputEvent notify functions
	*/
	static inline InputMap m_inputMap;
	//--------------------------------------
	WindowManager* m_windowMan;
private:
	/*
	* glfw callback functions
	*/
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mousePosCallback(GLFWwindow* window, double mouseXIn, double mouseYIn);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	/*
	* Engine event callbacks
	*/
	void mousePosHandler(float x, float y);
};

