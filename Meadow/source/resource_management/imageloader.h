#pragma once

#include <string>
#include <unordered_set>
//struct ImageData {
//	unsigned char* bytes;
//	int width, height, nrChannels;
//};

class ImageLoader
{
public:
	ImageLoader();
	bool loadImage(const std::string &path, int &width, int &height, std::vector<unsigned char> &bytes);
	~ImageLoader();
private:
	std::unordered_set<unsigned char*> m_loadedImages;
};