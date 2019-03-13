#pragma once
class Scene;
class CTempScene;
class CTempLoadScene;
class CTempInGameScene;
class CTempGameOverScene;
class CTempMenuScene;


class CSceneManager : public CSingleTonBase<CSceneManager>
{
	std::shared_ptr<Scene>  m_pTempScene;

	SceneState m_SceneType = MENU;
public:
	CSceneManager();
	~CSceneManager();
	std::shared_ptr<Scene> GetScene() { return m_pTempScene; }
	SceneState GetSceneType() { return m_SceneType; }
	void SetScene(SceneState state);
};

