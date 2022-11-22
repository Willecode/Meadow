#include "texture.h"
Texture::Texture(
	std::unique_ptr<std::vector<unsigned char>> img,
	unsigned int width,
	unsigned int height,
	Renderer::ImageFormat formatSource,
	Renderer::ImageFormat formatInternal,
	std::string name,
	bool mipmap
):
	m_img(std::move(img)), m_imgWidth(width), m_imgHeight(height),
	m_imgFormatSource(formatSource),
	m_imgFormatInternal(formatInternal),
	m_multisample(false),
	m_mipmap(mipmap),
	Asset(name)
{
}

Texture::Texture(unsigned int width, unsigned int height, bool multisample, std::string name, bool exposeToUI, bool depthmap):
	m_img(nullptr), m_imgWidth(width), m_imgHeight(height),
	Asset(name, exposeToUI),
	m_multisample(multisample)
{
	if (depthmap) {
		m_imgFormatSource = Renderer::ImageFormat::DEPTH;
		m_imgFormatInternal = Renderer::ImageFormat::DEPTH;
	}
	else
	{
		m_imgFormatSource = Renderer::ImageFormat::RGB;
		m_imgFormatInternal = Renderer::ImageFormat::RGB_HIGH_PRECISION;
	}
}

void Texture::loadToGPU()
{
	unsigned char* imgData = nullptr;
	if (m_img != nullptr)
		imgData = &(*m_img)[0];
	if (m_multisample)
		RendererLocator::getRenderer()->create2DTextureMS(getId(), m_imgWidth, m_imgHeight, m_imgFormatInternal);
	else
		RendererLocator::getRenderer()->create2DTexture(getId(), m_imgWidth, m_imgHeight, m_imgFormatSource, m_imgFormatInternal, imgData, m_mipmap);
}

void Texture::bindToSampler(const unsigned int& samplerId)
{
	if(m_multisample)
		RendererLocator::getRenderer()->bindTo2DSamplerMS(getId(), samplerId);
	else
		RendererLocator::getRenderer()->bindTo2DSampler(getId(), samplerId);
}

void Texture::deleteFromGPU()
{
	RendererLocator::getRenderer()->deleteTexture(getId());
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
