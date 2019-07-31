#pragma once
class CScene;
class CTempLoadScene;
class CTempInGameScene;
class CTempGameOverScene;
class CTempMenuScene;
#include <map>

class CSceneManager : public CSingleTonBase<CSceneManager>
{
	CScene*  m_pTempScene = NULL;
	CScene* m_pScene = NULL;
	SceneState m_SceneType = MENUSCENE;
	bool m_isModeSelected = false;
	bool m_isMapSelected = false;

public:
	bool GetSelectedMap() { return m_isMapSelected; }
	bool GetSelectedMode() { return m_isModeSelected; }
	void SetSelectedMode(bool mode) { m_isModeSelected = mode; }
	void SetSelectedMap(bool map) { m_isMapSelected = map; }
	
	ModNumber CheckModeButton(const POINT& pos);
	SceneState CheckMapButton(const POINT& pos);

	std::map<SceneState, CScene*> m_MapList;
	CSceneManager();
	~CSceneManager();
	SceneState GetSceneType() { return m_SceneType; }
	void SetScene(SceneState state);
};

