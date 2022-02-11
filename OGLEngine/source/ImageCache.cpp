#include "ImageCache.h"
#include <stb/stb_image.h>
#include <iostream>
ImageCache::ImageCache()
{
}

CacheData* ImageCache::loadImage(const std::string path)
{
    cmap::const_iterator pos = cacheMap.find(path);
    if (pos == cacheMap.end()) {
        std::cout << "loading image" << std::endl;
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "image loader failed" << std::endl;
            return nullptr;
        }
        else {
            CacheData cData;
            cData.dataPtr = data;
            cData.height = height;
            cData.width = width;
            cData.nrChannels = nrChannels;
            cacheVec.push_back(cData);
            cacheMap.insert({ path, &(cacheVec.back())});
            pos = cacheMap.find(path);
        }
    }
    return pos->second;
    //    MAP::const_iterator pos = map.find("string");
    //if (pos == map.end()) {
    //    //handle the error
    //}
    //else {
    //    std::string value = pos->second;
    //    ...
    //}
}

void ImageCache::freeData(const std::string path)
{
    stbi_image_free(cacheMap.at(path)->dataPtr);
}

void ImageCache::freeAllData()
{
    for (auto const& x : cacheMap) {
        freeData(x.first);
    }
}

