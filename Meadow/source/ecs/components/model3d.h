#pragma once
#include "assets/mesh.h"
struct Model3D{
	Mesh * mesh;
	bool wireframe;
	Model3D():mesh(nullptr), wireframe(false) {}
};