#pragma once
#include "benchmarkutils.h"
#include <chrono>
#include "service_locator/loggerlocator.h"

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
