#include "material.h"
#include "resource_management/shadermanager.h"
Material::Material(std::string name): Asset(name)
{
}
Material::AssetType Material::getAssetType()
{
	return AssetType::MATERIAL;
}

void Material::passToShader(ShaderManager* sdrMan)
{
	/*
	* Check textures assigned for this material. Set appropriate samplers.
	*/
	bool diffmap = false;
	bool specmap = false;
	bool opacmap = false;
	for (auto tex : m_textures) {
		if (tex.second != nullptr) {
			Locator::getRenderer()->bindTo2DSampler(tex.second->getId(), sdrMan->getTexSamplerId(tex.first));
			if (tex.first == Texture::TextureType::DIFFUSE_MAP) {
				m_uintPropsHidden["diffuse_map"] = sdrMan->getTexSamplerId(Texture::TextureType::DIFFUSE_MAP);
				diffmap = true;
			}
			else if (tex.first == Texture::TextureType::SPECULAR_MAP) {
				m_uintPropsHidden["specular_map"] = sdrMan->getTexSamplerId(Texture::TextureType::SPECULAR_MAP);
				specmap = true;
			}
			else if (tex.first == Texture::TextureType::OPACITY_MAP) {
				m_uintPropsHidden["opacity_map"] = sdrMan->getTexSamplerId(Texture::TextureType::OPACITY_MAP);
				opacmap = true;
			}
		}
	}
	setProperty("material.diffuse_map_present", diffmap);
	setProperty("material.specular_map_present", specmap);
	setProperty("material.opacity_map_present", opacmap);

	/*
	* Pass properties to shader as uniforms
	*/
	for (auto prop : m_boolPropsHidden)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_intPropsHidden)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_uintPropsHidden)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_floatPropsHidden)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec3PropsHidden)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_mat4PropsHidden)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);

	for (auto prop : m_boolPropsExposed)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_intPropsExposed)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_uintPropsExposed)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_floatPropsExposed)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_vec3PropsExposed)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);
	for (auto prop : m_mat4PropsExposed)
		sdrMan->setUniformDrawSpecific(prop.first, prop.second);

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

void Material::setTexture(Texture* tex, Texture::TextureType type)
{
	/*
	* nullptr disables the texturemap
	*/
	if (type == Texture::TextureType::DIFFUSE_MAP)
		setProperty("material.diffuse_map_present", !(tex == nullptr));
	else if (type == Texture::TextureType::SPECULAR_MAP)
		setProperty("material.specular_map_present", !(tex == nullptr));
	else if (type == Texture::TextureType::OPACITY_MAP)
		setProperty("material.opacity_map_present", !(tex == nullptr));
	m_textures[type] = tex;
}

Texture* Material::getTexture(Texture::TextureType type)
{
	auto it = m_textures.find(type);
	if (it != m_textures.end())
		return it->second;
	else
		return nullptr;
}

void Material::defaultPhong()
{
	clearProperties();
	setProperty("material.shininess", 10.f, true);
	setProperty("material.diffuse", MaterialConstants::DEFAULT_COLOR, true);
	setProperty("material.specular", MaterialConstants::DEFAULT_COLOR * 0.1f, true);
	setProperty("material.diffuse_map_present", false);
	setProperty("material.specular_map_present", false);
}

void Material::defaultColorOnlyMat()
{
	clearProperties();
	setProperty("color", MaterialConstants::DEFAULT_COLOR);
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

std::unordered_map<std::string, glm::mat4>* Material::getExposedPropertiesm4()
{
	return &m_mat4PropsExposed;
}

