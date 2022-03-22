#pragma once
#include <glad/glad.h>
#include <string>
#include "imagecache.h"
class Texture
{
public:
	Texture(std::string imagePath, GLenum target, ImageCache &cache):target(target)
	{
		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(target, id);
		// -----------------------
		// Set texture parameters here:
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// -----------------------
		// image loading
		ImageData* image = cache.loadImage(imagePath);
		if (image) {
			glTexImage2D(target, 0, GL_RGB, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, image->dataPtr);
		}
		glBindTexture(target, 0);
	}
	~Texture() {
		glDeleteTextures(1, &id);
	}
	void bind(GLuint textureUnit) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(target, id);
	}
private:
	GLuint id;
	GLenum target;
	
};

