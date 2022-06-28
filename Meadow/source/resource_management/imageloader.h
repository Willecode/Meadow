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
	bool loadImage(const std::string &path, int &width, int &height, Renderer::ImageFormat& format, std::vector<unsigned char> &bytes);
	~ImageLoader();
	void flipOnLoad(bool flip);
private:
	/*
	* Local cache
	*/
	std::unordered_set<unsigned char*> m_loadedImages;
};