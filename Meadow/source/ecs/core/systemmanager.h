#pragma once
#include "system.h"
#include "ecstypes.h"
#include <memory>
#include <unordered_map>

using namespace ECSTypes;

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> registerSystem()
	{
		const char* typeName = typeid(T).name();

		assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		m_systems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void setSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");

		m_signatures.insert({ typeName, signature });
	}


	void entityDestroyed(Entity entity);
	void entitySignatureChanged(Entity entity, Signature entitySignature);
private:
	std::unordered_map<const char*, Signature> m_signatures{};
	std::unordered_map<const char*, std::shared_ptr<System>> m_systems{};
};

