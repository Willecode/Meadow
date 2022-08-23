#pragma once
#include "lightsource.h"

/*
* Has a pretty funky linked list type memory management system. (should prolly be redesigned.)
* See HEAD, MAX_POINTLIGHT_COUNT and m_nextNode + constructor + destructor.
*/

class PointLight : public LightSource
{
public:
	PointLight();
	~PointLight();
	static void passAllInstancesToShader(ShaderManager* sdrMan);
	static bool maxInstanceCapacity();
	void resetToDefault() override;
private:
	static const unsigned int MAX_POINTLIGHT_COUNT = 30;
	static inline PointLight* HEAD = nullptr;
	PointLight* m_nextNode;
	std::map<LightSource::PropertyType, std::string> m_propTypeMap;
private:
	std::map<LightSource::PropertyType, std::string>* getPropTypeMap() override;
};

