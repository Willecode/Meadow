#pragma once
/*
* Base class for input events
*/
class InputEvent
{
public:
	const char* getType() {
		return returnType();
	}
private:
	virtual const char* returnType() {
		return "";
	};
};

class CameraUpEvent : public InputEvent
{
public:
	static constexpr const char*  EVENT_TYPE = "CameraUpEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class CameraDownEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "CameraDownEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class CameraForwardEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "CameraForwardEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class CameraBackwardEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "CameraBackwardEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class CameraLeftEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "CameraLeftEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class CameraRightEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "CameraRightEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class MouseMoveEvent : public InputEvent
{
public:
	float mouseX;
	float mouseY;
	static constexpr const char* EVENT_TYPE = "MouseMoveEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class CloseWindowEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "CloseWindowEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class MouseButtonLeftPressedEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "MouseButtonLeftPressedEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class MouseButtonLeftReleasedEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "MouseButtonLeftReleasedEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class MouseLockEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "MouseLockEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};

class MouseUnlockEvent : public InputEvent
{
public:
	static constexpr const char* EVENT_TYPE = "MouseUnlockEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};