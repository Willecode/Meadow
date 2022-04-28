#include "lightsource.h"
LightSource::LightSource(): m_valid(false), m_floatProperties(), m_vec3Properties(), m_id(0)
{
	for (int i = 0; i < LightSource::MAX_ID; i++) {
		if (idStatusArray[i]) {
			m_id = i;
			idStatusArray[i] = false;
			m_valid = true;
			break;
		}
	}
}

void LightSource::passToShader(ShaderManager* sdrMan)
{
	/*
	* Light data is constant throughout frame drawing
	*/
	for (auto prop : m_floatProperties)
		sdrMan->setFrameUniform(prop.first, prop.second);
	for (auto prop : m_vec3Properties)
		sdrMan->setFrameUniform(prop.first, prop.second);
	for (auto prop : m_intProperties)
		sdrMan->setFrameUniform(prop.first, prop.second);

}

void LightSource::setProperty(std::string name, float value)
{
	processPropertyName(name);
	m_floatProperties[name] = value;
}

void LightSource::setProperty(std::string name, int value)
{
	processPropertyName(name);
	m_intProperties[name] = value;
}

void LightSource::setProperty(std::string name, glm::vec3 value)
{
	processPropertyName(name);
	m_vec3Properties[name] = value;
}

void LightSource::clearProperties()
{
	m_vec3Properties.clear();
	m_floatProperties.clear();
}

bool LightSource::isValid()
{
	return m_valid;
}

void LightSource::defaultDirLight()
{
	clearProperties();
	setProperty("dirLight[].direction", glm::vec3(-1.0f));
	setProperty("dirLight[].ambient",  DEFAULT_LIGHT_COLOR * 0.2f);
	setProperty("dirLight[].diffuse",  DEFAULT_LIGHT_COLOR * 0.5f);
	setProperty("dirLight[].specular", DEFAULT_LIGHT_COLOR * 0.6f);
	setProperty("dirLightCount", 1);

}

void LightSource::defaultPointLight()
{
}

void LightSource::processPropertyName(std::string& prop)
{
	std::size_t t = prop.find("[]");
	if (t != std::string::npos) {
		prop.insert(t + 1, std::to_string(m_id));
	}

}
