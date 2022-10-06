#if 1
#pragma once
#include <memory>
#include <string>
#include "assets/mesh.h"
#include "imageloader.h"
#include "ecs/core/ecscoordinator.h"




namespace ModelImporting
{
	void objsFromFile(std::string path, ECSCoordinator* ecs);
}
#endif 