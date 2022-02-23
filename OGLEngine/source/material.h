#pragma once
#include <glm/vec3.hpp>
#include "texture.h"
#include "shader_s.h"
class Material
{
public:
	Material(glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float shininess,
		Texture* diffuseMap,
		Texture* specularMap):
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shininess(shininess),
		diffuseMap(diffuseMap),
		specularMap(specularMap)
	{}
	void passToShader(Shader* shader) {
		shader->setFloat3("material.diffuse",  diffuse);
		shader->setFloat3("material.ambient",  ambient);
		shader->setFloat3("material.specular", specular);
		shader->setFloat("material.shininess", shininess);
		shader->setInt("material.diffuse_map", 0);
		shader->setInt("material.specular_map", 1);
		diffuseMap->bind(0);
		specularMap->bind(1);
	}

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Texture* diffuseMap;
	Texture* specularMap;
	float shininess;
};

