#include "phongmaterial.h"


PhongMaterial::PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	diffuse(diffuse),
	specular(specular),
	shininess(shininess),
	diffuseMap(nullptr),
	specularMap(nullptr)
{}

PhongMaterial::PhongMaterial(std::shared_ptr<Texture> diffuseMap, std::shared_ptr<Texture> specularMap,
	float shininess):
	diffuse(DEFAULT_COLOR),
	specular(DEFAULT_COLOR),
	shininess(shininess),
	diffuseMap(diffuseMap),
	specularMap(specularMap)
{}

void PhongMaterial::passToShader(Shader * shader)
{
	// Old way for ref
	//-----
	//shader->setFloat("material.shininess", shininess);
	/*if (textured){
		shader->setInt("material.diffuse_map", 0);
		shader->setInt("material.specular_map", 1);
		diffuseMap->bind(0);
		specularMap->bind(1);
	}
	else {
		shader->setFloat3("material.diffuse", diffuse);
		shader->setFloat3("material.specular", specular);
	}*/
	//-----
	shader->setFloat ("material.shininess", shininess);
	shader->setFloat3("material.diffuse", diffuse);
	shader->setFloat3("material.specular", specular);
	if (diffuseMap != nullptr) {
		shader->setBool("material.diffuse_map_present", true);
		shader->setInt("material.diffuse_map", 0);
		diffuseMap->bind(0);
	}
	else {
		shader->setBool("material.diffuse_map_present", false);
	}
	if (specularMap != nullptr) {
		shader->setBool("material.specular_map_present", true);
		shader->setInt("material.specular_map", 1);
		specularMap->bind(1);
	}
	else {
		shader->setBool("material.specular_map_present", false);
	}
}
