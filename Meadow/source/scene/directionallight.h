#pragma once
#include "lightsource.h"
#include "map"
class DirectionalLight : public LightSource
{
public:
	DirectionalLight();
	~DirectionalLight();
	static void passAllInstancesToShader(ShaderManager* sdrMan);
	static bool maxInstanceCapacity();
	void resetToDefault() override;

private:
	static const unsigned int MAX_DIRLIGHT_COUNT = 5;
	static inline DirectionalLight* HEAD = nullptr;
	DirectionalLight* m_nextNode;
	std::map<LightSource::PropertyType, std::string> m_propTypeMap;
private:
	std::map<LightSource::PropertyType, std::string>* getPropTypeMap() override;
};

