#pragma once
#include "model.h"

class WorldEntity
{
public:
	WorldEntity();
	void update();

private:
	Model* m_graphics;
	//Controllercomponent m_controller;
	//PhysicsComponent m_physics;

};

