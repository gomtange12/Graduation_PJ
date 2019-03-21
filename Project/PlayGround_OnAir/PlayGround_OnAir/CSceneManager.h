#pragma once
class CTempScene;
class CTempLoadScene;
class CTempInGameScene;
class CTempGameOverScene;
class CTempMenuScene;


class CSceneManager : public CSingleTonBase<CSceneManager>
{
	CTempScene*  m_pTempScene = NULL;

	SceneState m_SceneType = MENUSCENE;
public:
	CSceneManager();
	~CSceneManager();
	SceneState GetSceneType() { return m_SceneType; }
	void SetScene(SceneState state);
};

