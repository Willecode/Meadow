#include "texture.h"
Texture::Texture(
	std::unique_ptr<std::vector<unsigned char>> img,
	unsigned int width,
	unsigned int height,
	Renderer::ImageFormat format
):
	m_img(std::move(img)), m_imgWidth(width), m_imgHeight(height), m_imgFormat(format), id(0)
{
}

void Texture::loadToGPU()
{
	Locator::getRenderer()->create2DTexture(id, m_imgWidth, m_imgHeight, m_imgFormat, &(*m_img)[0]);
}

void Texture::bindToSampler(const unsigned int& samplerId)
{
	Locator::getRenderer()->bindTo2DSampler(id, samplerId);
}

void Texture::deleteFromGPU()
{
	Locator::getRenderer()->deleteTexture(id);
}
