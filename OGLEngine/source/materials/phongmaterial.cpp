#include "phongmaterial.h"

PhongMaterial::PhongMaterial() :PhongMaterial(DEFAULT_COLOR, DEFAULT_COLOR, 0.75f * 128.0f) {}

PhongMaterial::PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	diffuse(diffuse),
	specular(specular),
	shininess(shininess),
	diffuseMap(nullptr),
	specularMap(nullptr),
	textured(false)
{}

PhongMaterial::PhongMaterial(Texture* diffuseMap, Texture* specularMap,
	float shininess):
	diffuse(DEFAULT_COLOR),
	specular(DEFAULT_COLOR),
	shininess(shininess),
	diffuseMap(diffuseMap),
	specularMap(specularMap),
	textured(true)
{}

void PhongMaterial::passToShader(Shader * shader)
{
	shader->setFloat("material.shininess", shininess);
	if (textured){
		shader->setInt("material.diffuse_map", 0);
		shader->setInt("material.specular_map", 1);
		diffuseMap->bind(0);
		specularMap->bind(1);
	}
	else {
		shader->setFloat3("material.diffuse", diffuse);
		shader->setFloat3("material.specular", specular);
	}
}
