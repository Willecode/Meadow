#pragma once
#include <memory>
#include "service_locator/locator.h"
#include "asset.h"
class Texture : public Asset
{
public:
	Texture(std::unique_ptr<std::vector<unsigned char>> img, unsigned int width, unsigned int height, Renderer::ImageFormat format);
	
	void loadToGPU(); // Generate buffers in graphics memory and push image texture data to them
	void bindToSampler(const unsigned int& samplerId);
	void deleteFromGPU();

	AssetType getAssetType() override;
private:
	
	std::unique_ptr<std::vector<unsigned char>> m_img;
	unsigned int m_imgWidth;
	unsigned int m_imgHeight;
	Renderer::ImageFormat m_imgFormat;
	
	
};

