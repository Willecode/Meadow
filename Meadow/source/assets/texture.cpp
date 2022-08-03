#include "texture.h"
Texture::Texture(
	std::unique_ptr<std::vector<unsigned char>> img,
	unsigned int width,
	unsigned int height,
	Renderer::ImageFormat formatSource,
	Renderer::ImageFormat formatInternal,
	std::string name
):
	m_img(std::move(img)), m_imgWidth(width), m_imgHeight(height),
	m_imgFormatSource(formatSource),
	m_imgFormatInternal(formatInternal),
	m_multisample(false),
	Asset(name)
{
}

Texture::Texture(unsigned int width, unsigned int height, bool multisample, std::string name, bool exposeToUI):
	m_img(nullptr), m_imgWidth(width), m_imgHeight(height),
	m_imgFormatSource(Renderer::ImageFormat::RGB),
	m_imgFormatInternal(Renderer::ImageFormat::RGB_HIGH_PRECISION),
	Asset(name, exposeToUI),
	m_multisample(multisample)
{
}

void Texture::loadToGPU()
{
	unsigned char* imgData = nullptr;
	if (m_img != nullptr)
		imgData = &(*m_img)[0];
	if (m_multisample)
		Locator::getRenderer()->create2DTextureMS(getId(), m_imgWidth, m_imgHeight);
	else
		Locator::getRenderer()->create2DTexture(getId(), m_imgWidth, m_imgHeight, m_imgFormatSource, m_imgFormatInternal, imgData);
}

void Texture::bindToSampler(const unsigned int& samplerId)
{
	if(m_multisample)
		Locator::getRenderer()->bindTo2DSamplerMS(getId(), samplerId);
	else
		Locator::getRenderer()->bindTo2DSampler(getId(), samplerId);
}

void Texture::deleteFromGPU()
{
	Locator::getRenderer()->deleteTexture(getId());
}

unsigned int Texture::getWidth()
{
	return m_imgWidth;
}

unsigned int Texture::getHeight()
{
	return m_imgHeight;
}

Texture::AssetType Texture::getAssetType()
{
	return AssetType::TEXTURE;
}
