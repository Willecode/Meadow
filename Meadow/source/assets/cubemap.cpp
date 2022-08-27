#include "cubemap.h"
#include "primitivecreation.h"
#include "resource_management/resourcemanager.h"
#include "service_locator/loggerlocator.h"
Cubemap::Cubemap(
	std::array<std::unique_ptr<std::vector<unsigned char>>, 6> images,
	int width, int height,
	std::string name):
	Asset(name), m_imgWidth(width), m_imgHeight(height), m_cubeId(0), m_images(std::move(images))
{
	/*for (int i = 0; i < images.size(); i++) {
		if (images[i] == nullptr) {
			LoggerLocator::getLogger()->getLogger()->error("Cubemap: got nullptr texture");
			return;
		}
		else
		{
			m_images[i] = std::move(images[i]);
		}
	}*/
}

void Cubemap::load()
{
	// Must be a valid asset
	if (getId() == 0) {
		LoggerLocator::getLogger()->getLogger()->error("Cubemap: tried to load with 0 id");
		return;
	}

	// Construct a raw image data ptr array
	std::array<unsigned char*, 6> imgData;
	for (int i = 0; i < m_images.size(); i++) {
		imgData[i] = &(*(m_images[i].get()))[0];
	}

	// Create a cube mesh
	auto cube = PrimitiveCreation::createCubeMesh();

	// Store it into resourceman
	ResourceManager resMan = ResourceManager::getInstance();
	m_cubeId = resMan.storeSubmesh(std::move(cube));

	RendererLocator::getRenderer()->createCubemap(getId());
	RendererLocator::getRenderer()->cubemapLoadTextures(getId(), imgData, m_imgWidth, m_imgHeight);
}

void Cubemap::draw(ShaderManager* sdrMan)
{
	// Set sampler
	sdrMan->setUniformDrawSpecific("skybox", sdrMan->getTexSamplerId(Texture::TextureType::CUBE_MAP));

	// Bind texture
	RendererLocator::getRenderer()->bindCubemap(getId());

	// Draw
	RendererLocator::getRenderer()->wireframe(false);
	RendererLocator::getRenderer()->faceCulling(false);
	auto resMan = ResourceManager::getInstance();
	resMan.getSubmesh(m_cubeId)->draw();
}

Asset::AssetType Cubemap::getAssetType()
{
	return AssetType::CUBEMAP;
}
