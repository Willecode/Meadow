#pragma once
#include "resource_management/shadermanager.h"
/*
* A Lightsource can only exist as an attribute of a SceneNode.
* Holds light source related shader uniforms can be forwarded to ShaderManager.
* 
* This class is kept quite flexible to ease shader development, so no locked in properties.
*/
class LightSource
{
public:
	LightSource();
	void passToShader(ShaderManager* sdrMan);
	void setProperty(std::string name, float value);
	void setProperty(std::string name, int value);
	void setProperty(std::string name, glm::vec3 value);
	void clearProperties();
	bool isValid();
	void defaultDirLight();
	void defaultPointLight();
private:
	static constexpr glm::vec3 DEFAULT_LIGHT_COLOR = glm::vec3(1.0f);
	/*
	* The properties set will be parsed and if "[]" found then change to "[<lightid>]"
	*/
	std::unordered_map<std::string, glm::vec3> m_vec3Properties;
	std::unordered_map<std::string, float> m_floatProperties;
	std::unordered_map<std::string, int> m_intProperties;
	
	/*
	* lights need unique id's.
	* if idStatusArray[i] == true then i is free.
	*/
	unsigned int m_id;
	static constexpr unsigned int MAX_ID = 49;
	static inline bool idStatusArray[MAX_ID] = {true};
	bool m_valid;
private:
	/*
	* parse prop and if "[]" found then change to "[<lightid>]
	*/
	void processPropertyName(std::string& prop);
};

