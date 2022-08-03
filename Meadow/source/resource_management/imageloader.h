#pragma once

#include <string>
#include <unordered_set>
#include "renderer/renderer.h"
//struct ImageData {
//	unsigned char* bytes;
//	int width, height, nrChannels;
//};

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	/*
	* path: path to file.
	* width, height: width and height of the image will be assigned to these references.
	* format: format of the image will be assigned to this reference.
	* bytes: image data will be assigned to this reference.
	*/
	bool loadImage(const std::string &path, int &width, int &height, Renderer::ImageFormat& format, std::vector<unsigned char> &bytes);
	void flipOnLoad(bool flip);
private:
	/*
	* Local cache
	*/
	std::unordered_set<unsigned char*> m_loadedImages;
};