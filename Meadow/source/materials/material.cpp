#include "material.h"
#include "resource_management/shadermanager.h"
Material::AssetType Material::getAssetType()
{
	return AssetType::MATERIAL;
}

void Material::passToShader(ShaderManager* sdrMan)
{
	for (auto prop : m_boolMap)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_intMap)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_floatMap)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec3Map)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_mat4Map)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
}

void Material::setProperty(std::string name, bool value) 
{
	m_boolMap.insert({ name, value });
}
void Material::setProperty(std::string name, int value) 
{
	m_intMap.insert({ name, value });
}
void Material::setProperty(std::string name, float value)
{
	m_floatMap.insert({ name, value });
}
void Material::setProperty(std::string name, glm::vec3 value)
{
	m_vec3Map.insert({ name, value });
}
void Material::setProperty(std::string name, glm::mat4 value)
{
	m_mat4Map.insert({ name, value });
}