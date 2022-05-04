#pragma once
#include <memory>
#include "assets/submesh.h"
class PrimitiveCreation
{
public:
	static std::unique_ptr<SubMesh> createCubeMesh();
	static std::unique_ptr<SubMesh> createSphere(int sectorCount, int stackCount);
};