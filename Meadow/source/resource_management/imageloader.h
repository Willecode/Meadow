#pragma once

#include <string>
#include <unordered_set>
struct ImageData {
	unsigned char* bytes;
	int width, height, nrChannels;
};

class ImageLoader
{
public:
	ImageLoader();
	ImageData loadImage(const std::string &path);
	~ImageLoader();
private:
	std::unordered_set<unsigned char*> m_loadedImages;
};