#pragma once
#include "assets/mesh.h"
#include <memory>
class PrimitiveCreation
{
public:
	static std::unique_ptr<Mesh> createCubeMesh();
	static std::unique_ptr<Mesh> createSphere(int sectorCount, int stackCount);
};