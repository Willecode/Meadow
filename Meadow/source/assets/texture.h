#pragma once
#include <memory>
#include "service_locator/rendererlocator.h"
#include "asset.h"
class Texture : public Asset
{
public:
	/*
	* This enum can be used by other classes to communicate how to use a texture.
	* The texture object itself doesn't use this.
	*/
	enum class TextureType {
		ALBEDO_MAP = 0,
		//SPECULAR_MAP,
		METALLIC_MAP,
		ROUGHNESS_MAP,
		AO_MAP,
		OPACITY_MAP,
		CUBE_MAP,
		NORMAL_MAP,
		SHADOW_MAP
	};
public:
	/*
	* Constructor for textures with imported image data
	*/
	Texture(std::unique_ptr<std::vector<unsigned char>> img, unsigned int width, unsigned int height,
		Renderer::ImageFormat formatSource, Renderer::ImageFormat formatInternal, std::string name = "Nameless texture", bool mipmap = true);
	
	/*
	* Use this constructor to create empty texture (no image data).
	* Used as attachments for framebuffers etc.
	* High precision.
	*/
	Texture(unsigned int width, unsigned int height, bool multisample, std::string name = "Nameless texture", bool exposeToUI = false, bool depthmap = false);

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
	bool m_multisample;
	bool m_mipmap;
};

