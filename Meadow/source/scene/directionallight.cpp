#include "directionallight.h"


DirectionalLight::DirectionalLight():
	m_nextNode(nullptr),
	m_propTypeMap()
{
	/*
	* Find an empty position in linked list of dirlights
	*/
	bool posFound = false;
	DirectionalLight* current = DirectionalLight::HEAD;
	if (current == nullptr) {
		HEAD = this;
		posFound = true;
	}
	else {
		for (int i = 0; i < MAX_DIRLIGHT_COUNT - 1; i++) {
			if (current->m_nextNode == nullptr) {
				current->m_nextNode = this;
				posFound = true;
				return;
			}
			else
				current = current->m_nextNode;
		}
	}

	/*
	* No position found, throw error >:)
	*/
	if (!posFound) {
		throw std::length_error("Tried to add too many dirlights");
	}

}

DirectionalLight::~DirectionalLight()
{
	/*
	* Remove from linked list
	*/
	DirectionalLight* current = DirectionalLight::HEAD;
	for (int i = 0; i < MAX_DIRLIGHT_COUNT; i++) {
		if (current->m_nextNode == this)
			current->m_nextNode = this->m_nextNode;
		else
			current = current->m_nextNode;
	}
}

void DirectionalLight::passAllInstancesToShader(ShaderManager* sdrMan)
{
	
	DirectionalLight* current = HEAD;
	int count = 0;
	for (int i = 0; i < MAX_DIRLIGHT_COUNT; i++) {
		if (current != nullptr) {
			current->passToShader(sdrMan, i);
			count++;
			current = current->m_nextNode;
		}

	}
	sdrMan->setFrameUniform("dirLightCount", count);
}

void DirectionalLight::resetToDefault()
{
	clearProperties();
	setProperty("dirLight[].direction", glm::vec3(-1.0f));
	setProperty("dirLight[].ambient",  DEFAULT_LIGHT_COLOR * 0.2f);
	setProperty("dirLight[].diffuse",  DEFAULT_LIGHT_COLOR * 0.5f);
	setProperty("dirLight[].specular", DEFAULT_LIGHT_COLOR * 0.6f);
}

bool DirectionalLight::maxInstanceCapacity()
{
	DirectionalLight* current = DirectionalLight::HEAD;
	if (current == nullptr)
		return false;
	for (int i = 0; i < MAX_DIRLIGHT_COUNT -1; i++) {
		if (current->m_nextNode == nullptr)
			return false;
		else
			current = current->m_nextNode;
	}
	return true;
}



std::map<LightSource::PropertyType, std::string>* DirectionalLight::getPropTypeMap()
{
	return &m_propTypeMap;
}

