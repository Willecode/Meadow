#include "imageloader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <service_locator/locator.h>
ImageLoader::ImageLoader() : m_loadedImages()
{
    stbi_set_flip_vertically_on_load(0);
}
bool ImageLoader::loadImage(const std::string& path, int& width, int& height, Renderer::ImageFormat& format, std::vector<unsigned char>& bytes)
{

    Locator::getLogger()->getLogger()->info("ImageLoader: Loading image {}\n", path.c_str()); // Logging


    int nrChannels;
    unsigned char* byteArr = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

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
    bytes.insert(bytes.end(), &byteArr[0], &byteArr[width*height*nrChannels]);

    /*
    * Determine format
    */
    if (nrChannels == 1)
        format = Renderer::ImageFormat::R;
    if (nrChannels == 3)
        format = Renderer::ImageFormat::RGB;
    if (nrChannels == 4)
        format = Renderer::ImageFormat::RGBA;

    m_loadedImages.insert(byteArr);
    return true;
}

ImageLoader::~ImageLoader()
{
    for (auto it = m_loadedImages.begin(); it != m_loadedImages.end(); it++)
    stbi_image_free(*it);
}

void ImageLoader::flipOnLoad(bool flip)
{
    if (flip)
        stbi_set_flip_vertically_on_load(1);
    if (!flip)
        stbi_set_flip_vertically_on_load(0);
}
