#pragma once
#include <unordered_map>
#include <array>
#include <glad\glad.h>

struct ImageData {
	unsigned char* dataPtr;
	int width, height, nrChannels;
	GLenum format;
};
class ImageCache
{
public:
	ImageCache();
	/*
	* Loads and returns queried data. Caches it for future use.
	imageName: name of the image to get
	width: pointer to where image width will be outputted
	height: pointer to where image height will be outputted
	nrChannels: pointer to where number of channels of the image will be outputted
	return:pointer to CacheData containing metadata and actual data
	*/
	ImageData* loadImage(const std::string path);
	void freeData(const std::string path);
	void freeAllData();

private:
	//// For storing cache data, for internal use
	//struct InternalCacheData {
	//	unsigned char* dataPtr;
	//	int width, height, nrChannels;
	//};
	int MAX_CACHE_SIZE;
	typedef std::unordered_map<std::string, ImageData*> cmap;
	cmap cacheMap;
	std::array<ImageData, 30> cache;
	int newestCacheIndex;
	typedef std::unordered_map<std::string, GLenum> formatMap;
	formatMap imageFormats;

	formatMap::const_iterator getFormatIt(std::string path);
};

