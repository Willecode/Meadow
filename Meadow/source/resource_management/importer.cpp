#if 0
#include "importer.h"
#include "resource_management/resourcemanager.h"
#include "resource_management/imageloader.h"
#include "service_locator/loggerlocator.h"
#include "resource_management/importutility.h"
#include "resource_management/modelimporting.h"

Importer::Importer(): m_scene(nullptr)
{
	InputEvents::importTextureREvent::subscribe(
		std::bind(&Importer::fileBrowserImportTextureR, this)
	);
	InputEvents::importTextureRGBEvent::subscribe(
		std::bind(&Importer::fileBrowserImportTextureRGB, this)
	);
	InputEvents::importTextureSRGBEvent::subscribe(
		std::bind(&Importer::fileBrowserImportTextureSRGB, this)
	);
	InputEvents::import3DModelEvent::subscribe(
		std::bind(&Importer::fileBrowserImport3DModel, this)
	);

}

void Importer::setScene(Scene* scene)
{
	m_scene = scene;
}

void Importer::importTexture(const std::string filepath, Renderer::ImageFormat toFmt)
{
	std::string pathCopy = filepath;
	fixPath(pathCopy);
	/*
	* Load file
	*/
	int width, height;
	auto vecptr = std::make_unique<std::vector<unsigned char>>();
	Renderer::ImageFormat fmt;
	ImageLoader loader;
	if (!loader.loadImage(pathCopy, width, height, fmt, *vecptr.get())) {
		LoggerLocator::getLogger()->getLogger()->error("Error: failed to import file ({}).", pathCopy);
		return;
	}

	/*
	* Create texture object
	*/
	auto texPtr = std::make_unique<Texture>(std::move(vecptr), width, height, fmt, toFmt, pathCopy);

	/*
	* Store it
	*/
	ResourceManager::getInstance().storeTexture(std::move(texPtr));
}

void Importer::import3DModel(const std::string path)
{
	if (!sceneProvided())
		return;
	std::string pathCopy = path;
	fixPath(pathCopy);

	ModelImporting::objsFromFile(pathCopy, m_scene, 0);
}

void Importer::fileBrowserImportTextureR()
{
	fileBrowserImportTexture(Renderer::ImageFormat::R);
}

void Importer::fileBrowserImportTextureRGB()
{
	fileBrowserImportTexture(Renderer::ImageFormat::RGB);
}

void Importer::fileBrowserImportTextureSRGB()
{
	fileBrowserImportTexture(Renderer::ImageFormat::sRGB);

}

void Importer::fileBrowserImportTexture(Renderer::ImageFormat toFmt)
{
	/*
	* Open file explorer, get path to the chosen file
	*/
	std::string filepath;
	if (!ImportUtility::fileBrowserOpenFile(filepath, ImportUtility::FileType::PNGJPG)) {
		LoggerLocator::getLogger()->getLogger()->info("File import interrupted.");
		return;
	}
	/*
	* Import file at path
	*/
	importTexture(filepath, toFmt);
}

void Importer::fileBrowserImport3DModel()
{
	/*
	* Open file explorer, get path to the chosen file
	*/
	std::string filepath;
	if (!ImportUtility::fileBrowserOpenFile(filepath, ImportUtility::FileType::OBJFBX)) {
		LoggerLocator::getLogger()->getLogger()->info("File import interrupted.");
		return;
	}
	/*
	* Import file at path
	*/
	import3DModel(filepath);
}

void Importer::fixPath(std::string& path)
{
	std::replace(path.begin(), path.end(), '\\', '/');
}

bool Importer::sceneProvided()
{
	if (m_scene == nullptr)
		return false;
	else
		return true;
}
#endif