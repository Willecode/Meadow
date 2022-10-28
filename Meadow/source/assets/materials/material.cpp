#include "material.h"
#include "shader/shaderuniformnamemap.h"
Material::AssetType Material::getAssetType()
{
	return AssetType::MATERIAL;
}

Material::Material(std::string name): Asset(name)
{
}

bool Material::operator==(const Material& other)
{
	return getId() == other.getId();
}

void Material::passToShader()
{
	/*
	* Set shader
	*/
	ShaderManager& sdrMan = ShaderManager::getInstance();
	sdrMan.bindShader(getShaderType());

	/*
	* Pass in textures
	*/
	ShaderUniformNameMap uniNames;
	for (auto tex : m_textures) {
		// Set sampler id property
		setProperty(uniNames.getTexMapName(tex.first), sdrMan.getTexSamplerId(tex.first));
		if (tex.second != nullptr) {
			// bind the tex
			RendererLocator::getRenderer()->bindTo2DSampler(tex.second->getId(), sdrMan.getTexSamplerId(tex.first));
			
			// Set tex present property
			setProperty(uniNames.getHasTexMapName(tex.first), true);
		}
		else {
			// Set tex missing property
			setProperty(uniNames.getHasTexMapName(tex.first), false);
		}
	}

	/*
	* Pass properties to shader as uniforms
	*/
	for (auto prop : m_boolPropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_intPropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_uintPropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_floatPropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec3PropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec4PropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_mat4PropsHidden)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);

	for (auto prop : m_boolPropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_intPropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_uintPropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_floatPropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec3PropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec4PropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_mat4PropsExposed)
		sdrMan.setUniformDrawSpecific(prop.first, prop.second);
}

void Material::setProperty(std::string name, bool value, bool expose)
{
	if (expose)
		m_boolPropsExposed[name] = value;
	else
		m_boolPropsHidden[name] = value;
}
void Material::setProperty(std::string name, int value, bool expose)
{
	if (expose)
		m_intPropsExposed[name] = value;
	else
		m_intPropsHidden[name] = value;
}
void Material::setProperty(std::string name, unsigned int value, bool expose)
{
	if (expose)
		m_uintPropsExposed[name] = value;
	else
		m_uintPropsHidden[name] = value;
}
void Material::setProperty(std::string name, float value, bool expose)
{
	if (expose)
		m_floatPropsExposed[name] = value;
	else
		m_floatPropsHidden[name] = value;
}
void Material::setProperty(std::string name, glm::vec3 value, bool expose)
{
	if (expose)
		m_vec3PropsExposed[name] = value;
	else
		m_vec3PropsHidden[name] = value;
}
void Material::setProperty(std::string name, glm::vec4 value, bool expose)
{
	if (expose)
		m_vec4PropsExposed[name] = value;
	else
		m_vec4PropsHidden[name] = value;
}
void Material::setProperty(std::string name, glm::mat4 value, bool expose)
{
	if (expose)
		m_mat4PropsExposed[name] = value;
	else
		m_mat4PropsHidden[name] = value;
}

void Material::clearProperties()
{
	m_boolPropsHidden.clear();
	m_intPropsHidden.clear();
	m_floatPropsHidden.clear();
	m_vec3PropsHidden.clear();
	m_mat4PropsHidden.clear();
}

std::unordered_map<std::string, bool>* Material::getExposedPropertiesb()
{
	return &m_boolPropsExposed;
}

std::unordered_map<std::string, int>* Material::getExposedPropertiesi()
{
	return &m_intPropsExposed;
}

std::unordered_map<std::string, unsigned int>* Material::getExposedPropertiesui()
{
	return &m_uintPropsExposed;
}

std::unordered_map<std::string, float>* Material::getExposedPropertiesf()
{
	return &m_floatPropsExposed;
}

std::unordered_map<std::string, glm::vec3>* Material::getExposedPropertiesv3()
{
	return &m_vec3PropsExposed;
}

std::unordered_map<std::string, glm::vec4>* Material::getExposedPropertiesv4()
{
	return &m_vec4PropsExposed;
}

std::unordered_map<std::string, glm::mat4>* Material::getExposedPropertiesm4()
{
	return &m_mat4PropsExposed;
}

