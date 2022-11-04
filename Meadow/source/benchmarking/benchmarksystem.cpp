#include "benchmarksystem.h"

#include "acomponent.h"
#include "bcomponent.h"
#include "service_locator/loggerlocator.h"
void BenchmarkSystem::init(ECSCoordinator* ecs)
{
	m_myVal = 0;
	m_ecs = ecs;
}

void BenchmarkSystem::update(float deltaT)
{
	for (auto& ent : m_entities) {
		AComponent& A = m_ecs->getComponent<AComponent>(ent);

		m_myVal += A.val;
	}
	//LoggerLocator::getLogger()->getLogger()->debug("Benchmark system value: {}", m_myVal);
	m_myVal = 0;
}
