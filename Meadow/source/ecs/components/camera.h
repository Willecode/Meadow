#pragma once

struct Camera {
	float fov, zNear, zFar;
	bool active;

	Camera(): fov(45.f), zNear(0.1f), zFar(100.f), active(false) {};
};