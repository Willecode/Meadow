#pragma once
#include "service_locator/locator.h"
#include "inputmap.h"
#include "dispatcher.h"
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
public:
	InputGather();
	void init(Dispatcher* disp, WindowManager* windowMan);
	void pollInputs();
	bool getInputFlag(InputFlag flag);
private:
	/*
	* These need to be static for them to be accessed in glfw callback functions, I think
	*/
	static inline InputMap m_inputMap;
	//static inline std::vector<InputMap> m_inputMaps;
	static inline Dispatcher* m_dispatcher;
	static inline bool initialized;
	static inline std::map<InputFlag, bool> m_inputFlags;

	WindowManager* m_windowMan;
private:
	/*
	* glfw callback functions
	*/
	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mousePosCallback(GLFWwindow* window, double mouseXIn, double mouseYIn);

};

