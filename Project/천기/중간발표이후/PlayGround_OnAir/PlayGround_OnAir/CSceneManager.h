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
public:
	std::map<SceneState, CScene*> m_MapList;
	CSceneManager();
	~CSceneManager();
	SceneState GetSceneType() { return m_SceneType; }
	void SetScene(SceneState state);
};

