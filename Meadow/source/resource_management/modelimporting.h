#pragma once
#include <memory>
#include <string>
#include "assets/mesh.h"
#include "imageloader.h"
#include "scene/scene.h"





namespace ModelImporting
{
	//std::shared_ptr<Model> importWavefront(std::string path, ImageCache &textureCache);
	void objsFromFile(std::string path, Scene* scene, unsigned int parentId);
}