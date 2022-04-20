#include "inputgather.h"
#include "inputevent.h"

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
}

void InputGather::init(Dispatcher* disp, WindowManager* windowMan)
{
	//glfwSetScrollCallback(window, &scrollCallback);
	m_windowMan = windowMan;
	/*
	* Register InputGather::callback as a keypress callback function
	*/
	glfwSetKeyCallback(m_windowMan->getWindow(), InputGather::keyCallback);
	glfwSetCursorPosCallback(m_windowMan->getWindow(), InputGather::mousePosCallback);
	glfwSetMouseButtonCallback(m_windowMan->getWindow(), InputGather::mouseButtonCallback);

	m_dispatcher = disp;
	/*
	* Create input map for input events
	*/
	std::map<std::pair<int, bool>, std::unique_ptr<InputEvent>> newMap;
	newMap[std::make_pair(GLFW_KEY_ESCAPE, true)] = std::make_unique<CloseWindowEvent>();
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_LEFT, true)] = std::make_unique<MouseButtonLeftPressedEvent>();
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_LEFT, false)] = std::make_unique<MouseButtonLeftReleasedEvent>();
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_RIGHT, true)] = std::make_unique<MouseLockEvent>();
	newMap[std::make_pair(GLFW_MOUSE_BUTTON_RIGHT, false)] = std::make_unique<MouseUnlockEvent>();
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

bool InputGather::getInputFlag(InputFlag flag)
{
	return m_inputFlags.at(flag);
}

void InputGather::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (initialized) {
		//Locator::getLogger()->getLogger()->info("Pressed key: {}", key);
		InputEvent* iEvent = m_inputMap.getEvent(key, action == GLFW_PRESS);
		if (iEvent != nullptr) {
			/*
			* We should construct an actual event here with an associated value.
			*/
			m_dispatcher->notify(iEvent->getType());
		}
	}

}

void InputGather::mousePosCallback(GLFWwindow* window, double mouseXIn, double mouseYIn)
{
	Locator::getLogger()->getLogger()->info("Mouse x:{} y:{}", mouseXIn, mouseYIn);
	m_dispatcher->notify2f(MouseMoveEvent::EVENT_TYPE, mouseXIn, mouseYIn);
}

void InputGather::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (initialized) {
		InputEvent* iEvent = m_inputMap.getEvent(button, action == GLFW_PRESS);
		if (iEvent != nullptr) {
			/*
			* We should construct an actual event here with an associated value.
			*/
			m_dispatcher->notify(iEvent->getType());
		}
	}
}
