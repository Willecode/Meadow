#pragma once

/*
* Stores various flags for UI. Subscribes to events affecting those flags.
*/
class UIFlags
{
public:
	UIFlags();

	/*
	* Public flags
	*/
	bool imguiStackToolVisible;

	/*
	* Event managed flag getters
	*/
	bool mainMenuVisible();
	bool assetWindowVisible();
	bool sceneGraphVisible();
	bool sceneNodeInspectorVisible();

private:
	bool m_showSceneGraph;
	bool m_showAssets;
	bool m_showMainMenu;
	bool m_showSceneNodeInspector;

	void setSceneGraphVisibility(bool f);
	void setAssetsVisibility(bool f);
	void setMainMenuVisibility(bool f);
	void setSceneNodeInspectorVisibility(bool f);
	void toggleMainMenuVisibility();
};

