#include "material.h"
#include "resource_management/shadermanager.h"
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

void Material::setTexture(Texture* tex, Texture::TextureType type)
{
	m_textures[type] = tex;
}
