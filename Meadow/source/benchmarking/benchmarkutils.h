#pragma once
#include "ecs/core/ecscoordinator.h"
#include "ecs/systems/scenegraphsystem/scenegraphsystem.h"
#include "assets/mesh.h"
namespace Benchmark {
	void addEntities(const unsigned int& count, ECSCoordinator* ecs);
	void addBenchmarkComponents(const unsigned int& count, ECSCoordinator* ecs);
	void addRigidBodySpheres(ECSCoordinator* ecs, Mesh* sphereMesh, SceneGraphSystem* sceneSystem);
}