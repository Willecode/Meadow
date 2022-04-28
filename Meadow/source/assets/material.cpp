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
	* Bind textures to appropriate samplers, set texture sampler properties
	*/
	for (auto tex : m_textures) {
		Locator::getRenderer()->bindTo2DSampler(tex.second->getId(), sdrMan->getTexSamplerId(tex.first));
		if (tex.first == Texture::TextureType::DIFFUSE_MAP)
			m_intMap["diffuse_map"] = sdrMan->getTexSamplerId(Texture::TextureType::DIFFUSE_MAP);
		else if (tex.first == Texture::TextureType::SPECULAR_MAP) {
			m_intMap["specular_map"] = sdrMan->getTexSamplerId(Texture::TextureType::SPECULAR_MAP);
		}
	}
	/*
	* Pass properties to shader as uniforms
	*/
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
	m_boolMap[name] = value;
}
void Material::setProperty(std::string name, int value) 
{
	m_intMap[name] = value;
}
void Material::setProperty(std::string name, float value)
{
	m_floatMap[name] = value;
}
void Material::setProperty(std::string name, glm::vec3 value)
{
	m_vec3Map[name] = value;
}
void Material::setProperty(std::string name, glm::mat4 value)
{
	m_mat4Map[name] = value;
}

void Material::clearProperties()
{
	m_boolMap.clear();
	m_intMap.clear();
	m_floatMap.clear();
	m_vec3Map.clear();
	m_mat4Map.clear();
}

void Material::setTexture(Texture* tex, Texture::TextureType type)
{
	m_textures[type] = tex;
}

void Material::defaultPhong()
{
	clearProperties();
	setProperty("material.shininess", 10.f);
	setProperty("material.diffuse", MaterialConstants::DEFAULT_COLOR);
	setProperty("material.specular", MaterialConstants::DEFAULT_COLOR * 0.1f);
	setProperty("material.diffuse_map_present", false);
	setProperty("material.specular_map_present", false);
}

void Material::defaultColorOnlyMat()
{
	clearProperties();
	setProperty("color", MaterialConstants::DEFAULT_COLOR);
}
