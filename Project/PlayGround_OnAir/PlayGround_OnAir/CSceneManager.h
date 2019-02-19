#pragma once
class CTempScene;
class CSceneManager : public CSingleTonBase< CSceneManager>
{
	shared_ptr <CTempScene> m_pTempScene = NULL;

public:
	CSceneManager();
	~CSceneManager();
	void SetScene(SceneState state);
};

