#include "phongmaterial.h"
#include "resource_management/shadermanager.h"
#include "../service_locator/locator.h"

PhongMaterial::PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	diffuse(diffuse),
	specular(specular),
	shininess(shininess),
	m_diffuseMap(nullptr),
	m_specularMap(nullptr),
	m_shader(ShaderManager::getPhongShader())

{}

PhongMaterial::PhongMaterial(std::shared_ptr<Texture> diffuseMap, std::shared_ptr<Texture> specularMap,
	float shininess):
	diffuse(MaterialConstants::DEFAULT_COLOR),
	specular(MaterialConstants::DEFAULT_COLOR * 0.1f),
	shininess(shininess),
	m_diffuseMap(diffuseMap),
	m_specularMap(specularMap),
	m_shader(ShaderManager::getPhongShader())
{}

void PhongMaterial::passToRenderer()
{
	Locator::getRenderer()->setFloat(m_shader->getId(),"material.shininess", shininess);
	Locator::getRenderer()->setFloat3(m_shader->getId(), "material.diffuse", diffuse);
	Locator::getRenderer()->setFloat3(m_shader->getId(), "material.specular", specular);
	if (m_diffuseMap != nullptr) {
		Locator::getRenderer()->setBool(m_shader->getId(), "material.diffuse_map_present", true);
		Locator::getRenderer()->setInt(m_shader->getId(), "material.diffuse_map", 0);
		/*
		* WTF **********************************************
		*/
		//m_diffuseMap->bind(0);
		/*
		* WTF **********************************************
		*/
	}
	else {
		Locator::getRenderer()->setBool(m_shader->getId(), "material.diffuse_map_present", false);
	}
	if (m_specularMap != nullptr) {
		Locator::getRenderer()->setBool(m_shader->getId(), "material.specular_map_present", true);
		Locator::getRenderer()->setInt(m_shader->getId(), "material.specular_map", 1);
		//specularMap->bind(1);
	}
	else {
		Locator::getRenderer()->setBool(m_shader->getId(), "material.specular_map_present", false);
	}
}
