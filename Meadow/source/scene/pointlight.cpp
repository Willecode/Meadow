#include "pointlight.h"
#include <stdexcept>
PointLight::PointLight():
m_nextNode(nullptr),
m_propTypeMap()
{
	/*
	* Find an empty position in linked list of dirlights
	*/
	bool posFound = false;
	PointLight* current = HEAD;
	if (current == nullptr) {
		HEAD = this;
		posFound = true;
	}
	else {
		for (int i = 0; i < MAX_POINTLIGHT_COUNT - 1; i++) {
			if (current->m_nextNode == nullptr) {
				current->m_nextNode = this;
				posFound = true;
				break;
			}
			else
				current = current->m_nextNode;
		}
	}

	/*
	* No position found, throw error >:)
	*/
	if (!posFound) {
		throw std::length_error("Tried to add too many pointlights");
	}
	resetToDefault();
	m_propTypeMap.insert({ PointLight::PropertyType::POSITION, "pointLights[].position" });
}

PointLight::~PointLight()
{
	/*
	* Remove from linked list
	*/
	if (this == HEAD) {
		HEAD = m_nextNode;
		return;
	}
	PointLight* current = HEAD;
	for (int i = 0; i < MAX_POINTLIGHT_COUNT; i++) {
		if (current->m_nextNode == this) {
			current->m_nextNode = this->m_nextNode;
			return;
		}
		else
			current = current->m_nextNode;
	}
}

void PointLight::passAllInstancesToShader(ShaderManager* sdrMan)
{
	PointLight* current = HEAD;
	int count = 0;
	for (int i = 0; i < MAX_POINTLIGHT_COUNT; i++) {
		if (current != nullptr) {
			current->passToShader(sdrMan, i);
			count++;
			current = current->m_nextNode;
		}
		else
			break;

	}
	sdrMan->setFrameUniform("pointLightCount", count);
}

bool PointLight::maxInstanceCapacity()
{
	PointLight* current = HEAD;
	if (current == nullptr)
		return false;
	for (int i = 0; i < MAX_POINTLIGHT_COUNT - 1; i++) {
		if (current->m_nextNode == nullptr)
			return false;
		else
			current = current->m_nextNode;
	}
	return true;
}

void PointLight::resetToDefault()
{
	clearProperties();
	setProperty("pointLights[].color", DEFAULT_LIGHT_COLOR);
	setProperty("pointLights[].constant", 1.0f);
	setProperty("pointLights[].linear", 0.09f);
	setProperty("pointLights[].quadratic", 0.032f);
}

std::map<LightSource::PropertyType, std::string>* PointLight::getPropTypeMap()
{
	return &m_propTypeMap;
}
