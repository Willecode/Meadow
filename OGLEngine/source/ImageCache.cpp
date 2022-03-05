#include "ImageCache.h"
#include <stb/stb_image.h>
#include <iostream>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
ImageCache::ImageCache(): newestCacheIndex(0), MAX_CACHE_SIZE(8), cache({})
{
}

ImageData* ImageCache::loadImage(const std::string path)
{
    cmap::const_iterator pos = cacheMap.find(path);
    if (pos == cacheMap.end()) {
        fmt::print(stdout, "Loading image {}\n", path);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            fmt::print(stdout, "stb failed to load image {}\n", path);
            return nullptr;
        }
        else {
            if (newestCacheIndex > MAX_CACHE_SIZE - 1) {
                std::cout << "cache full. loaded image not cached." << std::endl;
                return nullptr;
            }
            else {
                ImageData cData;
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
}

void ImageCache::freeData(const std::string path)
{
    stbi_image_free(cacheMap.at(path)->dataPtr);
}

void ImageCache::freeAllData()
{
    for (auto const& x : cacheMap) {
        stbi_image_free(cacheMap.at(x.first)->dataPtr);
    }
    cacheMap.clear();
    newestCacheIndex = 0;
}

