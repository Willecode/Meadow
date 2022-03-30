#pragma once
#include "mesh.h"
#include <memory>
class PrimitiveCreation
{
public:
	static std::shared_ptr<Mesh> createCubeMesh();
	static std::shared_ptr<Mesh> createSphere(int sectorCount, int stackCount);
};