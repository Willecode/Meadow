#include "ImageCache.h"
#include <stb/stb_image.h>
#include <iostream>
ImageCache::ImageCache(): newestCacheIndex(0), MAX_CACHE_SIZE(8)
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
            if (newestCacheIndex > MAX_CACHE_SIZE - 1) {
                std::cout << "cache full. loaded image not cached." << std::endl;
                return nullptr;
            }
            else {
                CacheData cData;
                cData.dataPtr = data;
                cData.height = height;
                cData.width = width;
                cData.nrChannels = nrChannels;
                cache[newestCacheIndex] = cData;
                cacheMap.insert({ path, &(cache.at(newestCacheIndex)) });
                newestCacheIndex++;
                pos = cacheMap.find(path);
            }
            
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

void ImageCache::freeAllData()
{
    for (auto const& x : cacheMap) {
        stbi_image_free(cacheMap.at(x.first)->dataPtr);
    }
    cacheMap.clear();
    newestCacheIndex = 0;
}

