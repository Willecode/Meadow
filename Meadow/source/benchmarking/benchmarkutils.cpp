#pragma once
#include "benchmarkutils.h"
#include <chrono>
#include "service_locator/loggerlocator.h"
#include "benchmarking/acomponent.h"
#include "benchmarking/bcomponent.h"
#include "ecs/components/rigidbody.h"
#include "ecs/components/transform.h"
#include "ecs/components/model3d.h"
#include "ecs/components/light.h"
#include "ecs/core/internalevents.h"
#include <stdlib.h>

void Benchmark::addEntities(const unsigned int& count, ECSCoordinator* ecs)
{
	LoggerLocator::getLogger()->getLogger()->debug("Adding {} entities", count);
	auto start = std::chrono::steady_clock::now();
	for (unsigned int i = 0; i < count; i++) {
		ecs->createEntity();
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	LoggerLocator::getLogger()->getLogger()->debug("Adding entities took {}s", elapsed_seconds.count());

}

void Benchmark::addBenchmarkComponents(const unsigned int& count, ECSCoordinator* ecs)
{
	for (unsigned int i = 0; i < count; i++) {
		Entity ent = ecs->createEntity();
		AComponent a(2);
		ecs->addComponent<AComponent>(ent, a);
	}
}

void Benchmark::addRigidBodySpheres(ECSCoordinator* ecs, Mesh* sphereMesh, SceneGraphSystem* sceneSystem)
{
	unsigned int wStart = 0;
	unsigned int wEnd = 40;
	float yCoord = 5; // Coords of the left bottom sphere
	float xCoord = -20;

	unsigned int rowAmount = 20;

	float gap = static_cast<float>((wEnd - wStart)) / static_cast<float>(rowAmount);
	std::vector<Entity> ents;
	for (float w = wStart; w <= wEnd; w += gap) { // Add entities
		for (float h = wStart; h <= wEnd; h += gap) {

			Entity ent = ecs->createEntity();
			Transform& t = ecs->getComponent<Transform>(ent);
			t.position.x = w + xCoord;
			t.position.y = h + yCoord;
			t.position.z += static_cast<float>(((rand() % 10) + 1)) / 20.f; // give some random z variance
			InternalEvents::MarkNodeTransformStaleEvent::notify(ent);
			ents.push_back(ent);

			Model3D m;
			m.mesh = sphereMesh;
			ecs->addComponent<Model3D>(ent, m); // Add sphere mesh component
		}

	}
	sceneSystem->update(); // Update stale transforms

	for (auto const& ent : ents) { // Add rigidbodies
		RigidBody r;
		r.type = RigidBody::RigidBodyType::DSPHERE;
		ecs->addComponent<RigidBody>(ent, r); // Add rigidbody component
	}
}

void Benchmark::adjustAllLights(ECSCoordinator* ecs)
{
	auto lightVec = ecs->getComponents<Light>();
	for (auto& light : lightVec) {
		light->color.r += 0.001;
	}
}
