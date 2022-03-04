#pragma once
#include "mesh.h"
#include <memory>
namespace PrimitiveCreation
{
	std::shared_ptr<Mesh> createCubeMesh();
	std::shared_ptr<Mesh> createSphere(int sectorCount, int stackCount);
}