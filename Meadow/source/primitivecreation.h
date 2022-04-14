#pragma once
#include <memory>
#include "assets/mesh.h"
class PrimitiveCreation
{
public:
	static std::unique_ptr<Mesh> createCubeMesh();
	static std::unique_ptr<Mesh> createSphere(int sectorCount, int stackCount);
};