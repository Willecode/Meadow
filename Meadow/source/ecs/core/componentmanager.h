#pragma once
#include "ecs/core/ecstypes.h"
#include "ecs/core/componentarray.h"
#include <unordered_map>
#include <memory>
using namespace ECSTypes;
class ComponentManager
{
public:

	template<typename T>
	void registerComponent() {
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once.");

		m_componentTypes.insert({ typeName, m_nextComponentType });
		m_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		++m_nextComponentType;
	}

	template<typename T>
	ComponentType getComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return m_componentTypes[typeName];
	}

	template<typename T>
	void addComponent(Entity entity, T component)
	{
		getComponentArray<T>()->insertComponent(entity, component);
	}

	template<typename T>
	void removeComponent(Entity entity)
	{
		getComponentArray<T>()->removeData(entity);
	}

	template<typename T>
	T& getComponent(Entity entity)
	{
		return getComponentArray<T>()->getData(entity);
	}

	template<typename T>
	std::vector<T*> getComponents()
	{
		return getComponentArray<T>()->getComponents();
	}
	void entityDestroyed(Entity entity);
private:
	std::unordered_map<const char*, ComponentType> m_componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays{};
	ComponentType m_nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	}
};

