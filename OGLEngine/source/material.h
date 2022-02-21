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
		Texture* diffuseMap,
		Texture* specularMap):
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		diffuseMap(diffuseMap),
		specularMap(specularMap)
	{}
	void passToShader(Shader* shader) {
		shader->setFloat3("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
		shader->setFloat3("material.ambient", ambient.r, ambient.g, ambient.b);
		shader->setFloat3("material.specular", specular.r, specular.g, specular.b);


	}

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Texture* diffuseMap;
	Texture* specularMap;
};

