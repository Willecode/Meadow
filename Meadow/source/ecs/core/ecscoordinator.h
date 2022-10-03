#pragma once
#include "componentmanager.h";
#include "entitymanager.h";
#include "systemmanager.h";
#include <memory>
class ECSCoordinator
{
public:
	ECSCoordinator();
	void init();
	Entity createEntity();
	void destroyEntity(Entity entity);
	Signature getEntitySignature(Entity ent) const;
	template<typename T>
	void registerComponent()
	{
		m_componentManager->registerComponent<T>();
	}

	template<typename T>
	void addComponent(Entity entity, T component)
	{
		m_componentManager->addComponent<T>(entity, component);

		auto signature = m_entityManager->getSignature(entity);
		signature.set(m_componentManager->getComponentType<T>(), true);
		m_entityManager->setSignature(entity, signature);

		m_systemManager->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	void removeComponent(Entity entity)
	{
		m_componentManager->removeComponent<T>(entity);

		auto signature = m_entityManager->getSignature(entity);
		signature.set(m_componentManager->getComponentType<T>(), false);
		m_entityManager->setSignature(entity, signature);

		m_systemManager->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& getComponent(Entity entity) const{
		return m_componentManager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentType getComponentType() const{
		return m_componentManager->getComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> registerSystem() {
		return m_systemManager->registerSystem<T>();
	}

	template<typename T>
	void setSystemSignature(Signature signature) {
		m_systemManager->setSignature<T>(signature);
	}

private:
	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
};
