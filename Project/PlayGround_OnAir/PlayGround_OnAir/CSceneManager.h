#pragma once
class CTempScene;
class CTempLoadScene;
class CTempInGameScene;
class CTempGameOverScene;
class CTempMenuScene;


class CSceneManager : public CSingleTonBase<CSceneManager>
{
	shared_ptr <CTempScene> m_pTempScene = NULL;

	SceneState m_SceneType = MENU;
public:
	CSceneManager();
	~CSceneManager();
	void SetScene(SceneState state);
};

