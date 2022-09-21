#pragma once
#include "assets/texture.h"
#include <unordered_map>
#include "shader/shadermanager.h"

namespace MaterialConstants {
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f);
}
class Material : public Asset
{
public:
	Material(std::string name = "Nameless material");
	bool operator==(const Material& other);
	AssetType getAssetType() override;

	void passToShader();
	/*
	* Texture map setter. Pass in nullptr to disable map.
	*/
	virtual void setTexture(Texture* tex, Texture::TextureType type) = 0;

	/*
	* Get all possible texture types and the set textures
	*/
	virtual std::unordered_map<Texture::TextureType, Texture*> getTextures() = 0;

	virtual ShaderManager::ShaderType getShaderType() = 0;

	/*
	* Material property setters.
	*/
	void setProperty(std::string name, bool value, bool expose = false);
	void setProperty(std::string name, int value, bool expose = false);
	void setProperty(std::string name, unsigned int value, bool expose = false);
	void setProperty(std::string name, float value, bool expose = false);
	void setProperty(std::string name, glm::vec3 value, bool expose = false);
	void setProperty(std::string name, glm::mat4 value, bool expose = false);
	void clearProperties();

	/*
	* Get exposed properties
	*/
	std::unordered_map<std::string, bool>* getExposedPropertiesb();
	std::unordered_map<std::string, int>* getExposedPropertiesi();
	std::unordered_map<std::string, unsigned int>* getExposedPropertiesui();
	std::unordered_map<std::string, float>* getExposedPropertiesf();
	std::unordered_map<std::string, glm::vec3>* getExposedPropertiesv3();
	std::unordered_map<std::string, glm::mat4>* getExposedPropertiesm4();
protected:

	/*
	* Material property maps. These will be passed to shadermanager as
	* uniforms when drawing an object with this material.
	* Exposed props are intended to be changed from outside the material class.
	*/
	std::unordered_map<std::string, bool>         m_boolPropsHidden;
	std::unordered_map<std::string, int>          m_intPropsHidden;
	std::unordered_map<std::string, unsigned int> m_uintPropsHidden;
	std::unordered_map<std::string, float>        m_floatPropsHidden;
	std::unordered_map<std::string, glm::vec3>    m_vec3PropsHidden;
	std::unordered_map<std::string, glm::mat4>    m_mat4PropsHidden;

	std::unordered_map<std::string, bool>         m_boolPropsExposed;
	std::unordered_map<std::string, int>          m_intPropsExposed;
	std::unordered_map<std::string, unsigned int> m_uintPropsExposed;
	std::unordered_map<std::string, float>        m_floatPropsExposed;
	std::unordered_map<std::string, glm::vec3>    m_vec3PropsExposed;
	std::unordered_map<std::string, glm::mat4>    m_mat4PropsExposed;

	std::unordered_map<Texture::TextureType, Texture*> m_textures;

};