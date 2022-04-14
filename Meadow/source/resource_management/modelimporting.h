#pragma once
#if 0
#include <memory>
#include <string>
#include "model.h"
#include "imageloader.h"




namespace ModelImporting
{
	std::shared_ptr<Model> importWavefront(std::string path, ImageCache &textureCache);
	std::shared_ptr<Model> objsFromFile(std::string path, ImageCache& textureCache);
}

#endif