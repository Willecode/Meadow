#include "inputgather.h"
#include "inputevents.h"
InputGather::InputGather():
	m_windowMan(nullptr)
{
	initialized = false;
	m_inputFlags[InputFlag::CameraUp] = false;
	m_inputFlags[InputFlag::CameraDown] = false;
	m_inputFlags[InputFlag::CameraForward] = false;
	m_inputFlags[InputFlag::CameraBackward] = false;
	m_inputFlags[InputFlag::CameraLeft] = false;
	m_inputFlags[InputFlag::CameraRight] = false;

	InputEvents::MouseMoveEvent::subscribe(std::bind(&InputGather::mousePosHandler, this, std::placeholders::_1, std::placeholders::_2));

}

void InputGather::init(WindowManager* windowMan)
{
	m_windowMan = windowMan;
	/*
	* Register handlers as glfw callbacks
	*/
	glfwSetKeyCallback(m_windowMan->getWindow(), InputGather::keyCallback);
	glfwSetCursorPosCallback(m_windowMan->getWindow(), InputGather::mousePosCallback);
	glfwSetMouseButtonCallback(m_windowMan->getWindow(), InputGather::mouseButtonCallback);
	//glfwSetScrollCallback(window, &scrollCallback);

	/*
	* Create input map for input events
	*/
	std::map<std::pair<int, bool>, std::function<void()>> newMap;
	newMap[std::make_pair(GLFW_KEY_ESCAPE, true)] = InputEvents::CloseWindowEvent::notify;
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_LEFT, true)] = InputEvents::MouseButtonLeftPressedEvent::notify;
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_LEFT, false)] = InputEvents::MouseButtonLeftReleasedEvent::notify;
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_RIGHT, true)] = InputEvents::MouseLockEvent::notify;
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_RIGHT, false)] = InputEvents::MouseUnlockEvent::notify;
	newMap[std::make_pair(GLFW_KEY_LEFT_ALT, true)] = InputEvents::MainMenuVisibilityToggleEvent::notify;
	newMap[std::make_pair(GLFW_KEY_P, true)] = InputEvents::PlayGameEvent::notify;
	newMap[std::make_pair(GLFW_KEY_O, true)] = InputEvents::StopGameEvent::notify;
	InputMap iMap(std::move(newMap));

	/*
	* Move the map into class member
	*/
	m_inputMap = std::move(iMap);

	/*
	* Inputgather is now initialized
	*/
	initialized = true;
}

void InputGather::pollInputs()
{
	GLFWwindow* window = m_windowMan->getWindow();
	m_inputFlags[InputFlag::CameraUp] = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	m_inputFlags[InputFlag::CameraDown] = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
	m_inputFlags[InputFlag::CameraForward] = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	m_inputFlags[InputFlag::CameraBackward] = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	m_inputFlags[InputFlag::CameraLeft] = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	m_inputFlags[InputFlag::CameraRight] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

bool InputGather::getInputFlag(InputFlag flag) const
{
	return m_inputFlags.at(flag);
}

void InputGather::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (initialized) {
		auto func = m_inputMap.getEventFunc(key, action == GLFW_PRESS);
		func();
	}

}

void InputGather::mousePosCallback(GLFWwindow* window, double mouseXIn, double mouseYIn)
{
	//LoggerLocator::getLogger()->getLogger()->info("Mouse x:{} y:{}", mouseXIn, mouseYIn);
	InputEvents::MouseMoveEvent::notify(mouseXIn, mouseYIn);
}

void InputGather::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (initialized) {
		auto func = m_inputMap.getEventFunc(button, action == GLFW_PRESS);
		func();
	}
}

void InputGather::mousePosHandler(float x, float y)
{
	lastFrameMouseX = mouseX;
	lastFrameMouseY = mouseY;
	mouseX = x;
	mouseY = y;
}
