#include "ImageCache.h"
#include <stb/stb_image.h>
#include <iostream>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
ImageCache::ImageCache():
    newestCacheIndex(0),
    MAX_CACHE_SIZE(30),
    cache({}),
    imageFormats({ {"jpg", GL_RGB}, {"png", GL_RGBA}})
{
}

ImageData* ImageCache::loadImage(const std::string path)
{
    formatMap::const_iterator it = getFormatIt(path);
    if (it == imageFormats.end()) {
        fmt::print("ERROR: File {} is not of a suitable format.\n", path);
        return nullptr;
    }
    cmap::const_iterator pos = cacheMap.find(path);
    if (pos == cacheMap.end()) {
        fmt::print(stdout, "Loading image {}\n", path);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            fmt::print(stdout, "ImageCache: stb failed to load image {}, returning nullptr\n", path);
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
                cData.format = it->second;

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

ImageCache::formatMap::const_iterator ImageCache::getFormatIt(std::string path)
{
    std::size_t dotPos = path.find_last_of(".");
    if (dotPos != std::string::npos) {
        std::string suffix = path.substr(dotPos + 1, (path.length()) - dotPos);
        return imageFormats.find(suffix);
    }
    else
    {
        fmt::print("ERROR: File {} does not have a suffix.", path);
        return imageFormats.end();
    }

}



