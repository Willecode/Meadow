#include "lightsource.h"
LightSource::LightSource():m_floatProperties(), m_vec3Properties()
{

}

void LightSource::setProperty(std::string name, float value)
{
	m_floatProperties[name] = value;
}

void LightSource::setProperty(std::string name, int value)
{
	m_intProperties[name] = value;
}

void LightSource::setProperty(std::string name, glm::vec3 value)
{
	m_vec3Properties[name] = value;
}

void LightSource::setProperty(PropertyType name, float value)
{
	auto it = getPropTypeMap()->find(name);
	if (it != getPropTypeMap()->end())
		setProperty(it->second, value);
}

void LightSource::setProperty(PropertyType name, int value)
{
	auto it = getPropTypeMap()->find(name);
	if (it != getPropTypeMap()->end())
		setProperty(it->second, value);
}

void LightSource::setProperty(PropertyType name, glm::vec3 value)
{
	auto it = getPropTypeMap()->find(name);
	if (it != getPropTypeMap()->end())
		setProperty(it->second, value);
}

void LightSource::clearProperties()
{
	m_vec3Properties.clear();
	m_floatProperties.clear();
}

std::string LightSource::injectId(const std::string &prop, int id)
{
	std::string ret = prop;
	std::size_t t = ret.find("[]");
	if (t != std::string::npos) {
		ret.insert(t + 1, std::to_string(id));
	}
	return ret;
}

void LightSource::passToShader(ShaderManager* sdrMan, int id)
{
	/*
* Light data is constant throughout frame drawing
*/
	for (auto& prop : m_floatProperties)
		sdrMan->setFrameUniform(injectId(prop.first, id), prop.second);
	for (auto prop : m_vec3Properties)
		sdrMan->setFrameUniform(injectId(prop.first, id), prop.second);
	for (auto prop : m_intProperties)
		sdrMan->setFrameUniform(injectId(prop.first, id), prop.second);
}
