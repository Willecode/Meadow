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
	virtual const char* returnType() = 0;
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
	static constexpr const char* EVENT_TYPE = "MouseMoveEvent";
private:
	const char* returnType() override {
		return EVENT_TYPE;
	}
};
