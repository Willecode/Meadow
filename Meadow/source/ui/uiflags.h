#pragma once

/*
* Stores various flags for UI. Subscribes to events affecting those flags.
*/
class UIFlags
{
public:
	UIFlags();

	bool mainMenuVisible();
	bool assetWindowVisible();
	bool sceneGraphVisible();

private:
	bool m_showSceneGraph;
	bool m_showAssets;
	bool m_showMainMenu;

	void setSceneGraphVisibility(bool f);
	void setAssetsVisibility(bool f);
	void setMainMenuVisibility(bool f);
	void toggleMainMenuVisibility();
};

