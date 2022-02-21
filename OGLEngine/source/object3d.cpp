#include "Object3D.h"
#include <glm/gtc/type_ptr.hpp>
Object3D::Object3D(): modelMatrix(glm::mat4(1.0f))
{
}

void Object3D::draw()
{
	if (material && shader) {
		material->passToShader(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].draw();
		}
	}
	else
		std::cout << "Tried to draw object without assigning shader and material\n";
	
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