#include "imageloader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <service_locator/locator.h>
ImageLoader::ImageLoader() : m_loadedImages()
{
    stbi_set_flip_vertically_on_load(1);
}
ImageData ImageLoader::loadImage(const std::string& path)
{

    Locator::getLogger()->getLogger()->info("ImageLoader: Loading image {}\n", path.c_str()); // Logging

    /*
    * if stbi_load() fails, it returns nullptr
    */
    int width, height, nrChannels;
    unsigned char* bytes = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (stbi_failure_reason())
        Locator::getLogger()->getLogger()->info("ImageLoader: detected failure: {}\n", stbi_failure_reason()); // Logging
    if (!bytes) {
        Locator::getLogger()->getLogger()->info("ImageLoader: stb failed to load image {}, returning byte ptr as nullptr\n", path.c_str()); // Logging
    }
    /*
    * return image data
    */
    ImageData img;
    img.bytes = bytes;
    img.height = height;
    img.width = width;
    img.nrChannels = nrChannels;

    m_loadedImages.insert(img.bytes);
    return img;
}

ImageLoader::~ImageLoader()
{
    for (auto it = m_loadedImages.begin(); it != m_loadedImages.end(); it++)
    stbi_image_free(*it);
}
