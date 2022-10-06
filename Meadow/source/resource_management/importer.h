#if 1
#pragma once
#include "ecs/core/ecscoordinator.h"
#include "renderer/renderer.h"
/*
* Imports files into the program.
* This includes:
*	* 3D model files (fbx, obj)
*	* Image files (png, jpg)
*/
class Importer
{
public:
	Importer();
	void init(ECSCoordinator* ecs);

	void importTexture(const std::string path, Renderer::ImageFormat toFmt);
	void import3DModel(const std::string path);
	
	void fileBrowserImportTextureR();
	void fileBrowserImportTextureRGB();
	void fileBrowserImportTextureSRGB();

	void fileBrowserImport3DModel();

private:
	/*
	* Replace path's "\\" with "/"
	*/
	void fixPath(std::string& path);

	void fileBrowserImportTexture(Renderer::ImageFormat toFmt);
	ECSCoordinator* m_ecs;
	bool m_initialized;
};

#endif