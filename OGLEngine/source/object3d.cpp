#include "Object3D.h"
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp> // vec/mat debug prints
Object3D::Object3D(): 
	modelMatrix(glm::mat4(1.0f)),
	name("none"),
	id(-1)
{
}

void Object3D::draw(std::unordered_map<Object3D*, LightSource*> sceneLights)
{
	if ((!meshes.empty()) && material && shader) {
		shader->use();
		material->passToShader(shader);
		// THIS DOESN'T SUPPORT DIRLIGHTS
		GLuint lightId = 0;
		for (auto const& x : sceneLights) {
			if (!(x.first->id == id)) {
				x.second->passToShader(shader, lightId, x.first->getPosition());
				lightId++;
			}
		}
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i]->draw();
		}
	}
	else
		std::cout << "Tried to draw object without assigning shader and material\n";
	
}

void Object3D::addLightSource(LightSource* lightSrc)
{
	light = lightSrc;
}

void Object3D::setModelMatrix(const glm::mat4* model)
{
	modelMatrix = *model;
}

void Object3D::setMaterial(Material* mat)
{
	material = mat;
}

void Object3D::setShader(Shader* sdr)
{
	shader = sdr;
}

void Object3D::addMesh(const Mesh* mesh)
{
	meshes.push_back(mesh);
}

LightSource* Object3D::getLightSource()
{
	return light;
}

glm::vec3 Object3D::getPosition()
{
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
	return glm::vec3(0.0f, 0.0f, 0.0f);
}
