#pragma once
#include "../texture.h"
#include "resource_management/shadermanager.h"


namespace MaterialConstants {
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f);
}
class Material : public Asset
{
public:
	AssetType getAssetType() override;
	void passToShader(ShaderManager* sdrMan);
	
	/*
	* Material property setters.
	*/
	void setProperty(std::string name, bool value);
	void setProperty(std::string name, int value);
	void setProperty(std::string name, float value);
	void setProperty(std::string name, glm::vec3 value);
	void setProperty(std::string name, glm::mat4 value);

	/*
	* Texture map setter. Pass in nullptr to disable map.
	*/
	void setTexture(Texture* tex, Texture::TextureType type);
private:
	/*
	* Material property maps. These will be passed to shadermanager as 
	* uniforms when drawing an object with this material.
	*/
	std::unordered_map<std::string, bool> m_boolMap;
	std::unordered_map<std::string, int> m_intMap;
	std::unordered_map<std::string, float> m_floatMap;
	std::unordered_map<std::string, glm::vec3> m_vec3Map;
	std::unordered_map<std::string, glm::mat4> m_mat4Map;

	std::unordered_map<Texture::TextureType, Texture*> m_textures;
};