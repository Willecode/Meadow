#if 0
#pragma once
#include "scene/scene.h"
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
	void setScene(Scene* scene);

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

	/*
	* Check whether scene has been provided
	*/
	bool sceneProvided();

	void fileBrowserImportTexture(Renderer::ImageFormat toFmt);
	Scene* m_scene;
};

#endif