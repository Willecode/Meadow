#pragma once
#if 0
#include <memory>
#include "model.h"
#include <string>
#include "imageloader.h"




namespace ModelImporting
{
	std::shared_ptr<Model> importWavefront(std::string path, ImageCache &textureCache);
	std::shared_ptr<Model> objsFromFile(std::string path, ImageCache& textureCache);
}

#endif