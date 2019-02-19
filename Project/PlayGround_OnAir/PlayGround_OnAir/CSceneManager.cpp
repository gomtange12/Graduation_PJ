#include "stdafx.h"
#include "CSceneManager.h"
#include "Scene.h"

CSceneManager::CSceneManager()
{
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
		m_pTempScene = new CTempLoadScene;
		m_SceneType = E_MENU;

		break;
	case INGAME:
		m_pTempScene = new CMyInGame;
		m_SceneType = E_INGAME;

		break;
	case GAMEOVER:
		m_pTempScene = new CMyGameOver;
		m_SceneType = E_GAMEOVER;

		break;

	}*/
}
