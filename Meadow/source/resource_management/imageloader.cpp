#include "imageloader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <service_locator/locator.h>
ImageLoader::ImageLoader() : m_loadedImages()
{
    stbi_set_flip_vertically_on_load(0);
}
bool ImageLoader::loadImage(const std::string& path, int& width, int& height, std::vector<unsigned char>& bytes)
{

    Locator::getLogger()->getLogger()->info("ImageLoader: Loading image {}\n", path.c_str()); // Logging

    /*
    * Force stb to import with 4 channels (all textures will be rgba)
    */
    int nrChannels;
    unsigned char* byteArr = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

    /*
    * if stbi_load() fails, it returns false
    */
    if (stbi_failure_reason())
        Locator::getLogger()->getLogger()->info("ImageLoader: detected failure: {}\n", stbi_failure_reason()); // Logging
    if (!byteArr) {
        Locator::getLogger()->getLogger()->info("ImageLoader: stb failed to load image {}, returning false\n", path.c_str()); // Logging
        return false;
    }

    /*
    * Populate provided vec with bytes
    */
    bytes.clear();
    bytes.insert(bytes.end(), &byteArr[0], &byteArr[width*height*4]);

    m_loadedImages.insert(byteArr);
    return true;
}

ImageLoader::~ImageLoader()
{
    for (auto it = m_loadedImages.begin(); it != m_loadedImages.end(); it++)
    stbi_image_free(*it);
}
