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

	void importTexture(const std::string path);
	void import3DModel(const std::string path);
	
	void fileBrowserImportTexture();
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
	Scene* m_scene;
};

