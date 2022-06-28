#pragma once
#include <memory>
#include "service_locator/locator.h"
#include "asset.h"
class Texture : public Asset
{
public:
	/*
	* This enum can be used by other classes to communicate how to use a texture.
	* The texture object itself doesn't use this.
	*/
	enum class TextureType {
		DIFFUSE_MAP = 0,
		SPECULAR_MAP,
		OPACITY_MAP,
		CUBE_MAP
	};
public:
	Texture(std::unique_ptr<std::vector<unsigned char>> img, unsigned int width, unsigned int height,
		Renderer::ImageFormat formatSource, Renderer::ImageFormat formatInternal, std::string name = "Nameless texture");
	
	/*
	* Use this constructor to create empty texture (no image data)
	*/
	Texture(unsigned int width, unsigned int height, std::string name = "Nameless texture");

	void loadToGPU(); // Generate buffers in graphics memory and push image texture data to them
	void bindToSampler(const unsigned int& samplerId);
	void deleteFromGPU();

	unsigned int getWidth();
	unsigned int getHeight();

	AssetType getAssetType() override;
private:
	
	std::unique_ptr<std::vector<unsigned char>> m_img;
	unsigned int m_imgWidth;
	unsigned int m_imgHeight;
	Renderer::ImageFormat m_imgFormatSource;
	Renderer::ImageFormat m_imgFormatInternal;
};

