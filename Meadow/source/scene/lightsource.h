#pragma once
#include "resource_management/shadermanager.h"
#include <map>
/*
* A Lightsource can only exist as an attribute of a SceneNode.
* Holds light source related shader uniforms can be forwarded to ShaderManager.
* 
* This class is kept quite flexible to ease shader development, so no locked in properties.
*/

class LightSource
{
public:
	enum class PropertyType {
		POSITION = 0
	};
public:
	LightSource();
	virtual ~LightSource() = default;
	void setProperty(std::string name, float value);
	void setProperty(std::string name, int value);
	void setProperty(std::string name, glm::vec3 value);
	void setProperty(PropertyType name, float value);
	void setProperty(PropertyType name, int value);
	void setProperty(PropertyType name, glm::vec3 value);
	void clearProperties();

	virtual void resetToDefault() = 0;
protected:
	static constexpr glm::vec3 DEFAULT_LIGHT_COLOR = glm::vec3(1.0f);
	/*
	* The properties set will be parsed and if "[]" found then change to "[<lightid>]"
	*/
	std::unordered_map<std::string, glm::vec3> m_vec3Properties;
	std::unordered_map<std::string, float> m_floatProperties;
	std::unordered_map<std::string, int> m_intProperties;

protected:
	/*
	* parse prop and if "[]" found then change to "[<lightid>]
	*/
	std::string injectId(const std::string& prop, int id);
	void passToShader(ShaderManager* sdrMan, int id);
private:
	virtual std::map<LightSource::PropertyType, std::string>* getPropTypeMap() = 0;
};

