#include "uiflags.h"
#include "input/inputevents.h"

UIFlags::UIFlags():
	m_showSceneGraph(true),
	m_showAssets(true),
	m_showMainMenu(true),
	m_showSceneNodeInspector(true)
{
	InputEvents::MainMenuVisibilityToggleEvent::subscribe(std::bind(&UIFlags::toggleMainMenuVisibility, this));
	InputEvents::AssetWindowVisibilityEvent::subscribe(std::bind(&UIFlags::setAssetsVisibility, this, std::placeholders::_1));
	InputEvents::SceneGraphVisibilityEvent::subscribe(std::bind(&UIFlags::setSceneGraphVisibility, this, std::placeholders::_1));
	InputEvents::SceneNodeInspectorVisibilityEvent::subscribe(std::bind(&UIFlags::setSceneNodeInspectorVisibility, this, std::placeholders::_1));
}

bool UIFlags::mainMenuVisible()
{
	return m_showMainMenu;
}

bool UIFlags::assetWindowVisible()
{
	return m_showAssets;
}

bool UIFlags::sceneGraphVisible()
{
	return m_showSceneGraph;
}

bool UIFlags::sceneNodeInspectorVisible()
{
	return m_showSceneNodeInspector;
}

void UIFlags::setSceneGraphVisibility(bool f)
{
	m_showSceneGraph = f;
}

void UIFlags::setAssetsVisibility(bool f)
{
	m_showAssets = f;
}

void UIFlags::setMainMenuVisibility(bool f)
{
	m_showMainMenu = f;
}

void UIFlags::setSceneNodeInspectorVisibility(bool f)
{
	m_showSceneNodeInspector = f;
}

void UIFlags::toggleMainMenuVisibility()
{
	m_showMainMenu = !m_showMainMenu;
}
