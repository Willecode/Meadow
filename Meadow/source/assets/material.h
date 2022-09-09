#pragma once
#include "texture.h"
#include "resource_management/shadermanager.h"


namespace MaterialConstants {
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f);
}
class Material : public Asset
{
public:
	Material(std::string name = "Nameless material");
	AssetType getAssetType() override;
	void passToShader(ShaderManager* sdrMan);
	
	/*
	* Material property setters.
	*/
	void setProperty(std::string name, bool value, bool expose = false);
	void setProperty(std::string name, int value, bool expose = false);
	void setProperty(std::string name, float value, bool expose = false);
	void setProperty(std::string name, glm::vec3 value, bool expose = false);
	void setProperty(std::string name, glm::mat4 value, bool expose = false);
	void clearProperties();

	/*
	* Texture map setter. Pass in nullptr to disable map.
	*/
	void setTexture(Texture* tex, Texture::TextureType type);
	/*
	* Get currently assigned texture
	*/
	Texture* getTexture(Texture::TextureType type);
	/*
	* Functions to reset material as a default material (function for each shader type)
	*/
	void defaultPhong();
	void defaultColorOnlyMat();
	void defaultPBR();

	/*
	* Get exposed properties
	*/
	std::unordered_map<std::string, bool>* getExposedPropertiesb();
	std::unordered_map<std::string, int>* getExposedPropertiesi();
	std::unordered_map<std::string, unsigned int>* getExposedPropertiesui();
	std::unordered_map<std::string, float>* getExposedPropertiesf();
	std::unordered_map<std::string, glm::vec3>* getExposedPropertiesv3();
	std::unordered_map<std::string, glm::mat4>* getExposedPropertiesm4();

	

private:
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