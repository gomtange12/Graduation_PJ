#include "stdafx.h"

#include "CSceneManager.h"
#include "CTempScene.h"
CSceneManager::CSceneManager()
{
	m_pTempScene = std::make_shared<CTempScene>();

}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::SetScene(SceneState state)
{
	/*if (m_pTempScene)
	{
		delete m_pTempScene;
		m_pScene = NULL;
	}*/
	/*switch (state)
	{
	case MENU:
		
		m_pTempScene = std::make_shared<CTempLoadScene>();

		m_SceneType = MENU;

		break;
	case INGAME:
		m_pTempScene = std::make_shared<CTempInGameScene>();
		m_SceneType = INGAME;

		break;
	case GAMEOVER:
		m_pTempScene = std::make_shared<CTempGameOverScene>();
		m_SceneType = GAMEOVER;

		break;

	}*/
}
