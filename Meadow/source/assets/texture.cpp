#include "texture.h"
Texture::Texture(
	std::unique_ptr<std::vector<unsigned char>> img,
	unsigned int width,
	unsigned int height,
	Renderer::ImageFormat formatSource,
	Renderer::ImageFormat formatInternal,
	std::string name
):
	m_img(std::move(img)), m_imgWidth(width), m_imgHeight(height), m_imgFormatSource(formatSource), m_imgFormatInternal(formatInternal), Asset(name)
{
}

void Texture::loadToGPU()
{
	Locator::getRenderer()->create2DTexture(getId(), m_imgWidth, m_imgHeight, m_imgFormatSource, m_imgFormatInternal, &(*m_img)[0]);
}

void Texture::bindToSampler(const unsigned int& samplerId)
{
	Locator::getRenderer()->bindTo2DSampler(getId(), samplerId);
}

void Texture::deleteFromGPU()
{
	Locator::getRenderer()->deleteTexture(getId());
}

Texture::AssetType Texture::getAssetType()
{
	return AssetType::TEXTURE;
}
