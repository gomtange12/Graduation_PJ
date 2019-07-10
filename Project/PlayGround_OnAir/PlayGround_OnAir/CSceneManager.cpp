#include "stdafx.h"

#include "CSceneManager.h"
#include "CTempScene.h"
#include "Scene.h"

ModNumber CSceneManager::CheckModeButton(const POINT & pos)
{
	XMFLOAT2 cursorpos{ 2.0f * (static_cast<float>(pos.x) / static_cast<float>(FRAME_BUFFER_WIDTH)) - 1.0f
					, -(2.0f * (static_cast<float>(pos.y) / static_cast<float>(FRAME_BUFFER_HEIGHT)) - 1.0f) };

	if (cursorpos.x > 0.25 && cursorpos.x < 0.55 && cursorpos.y>0.15 && cursorpos.y < 0.35)
	{
		cout << "¼Ö·Î ¼±ÅÃ" << endl;
		return SOLO;
	}
	else if (cursorpos.x > 0.55 && cursorpos.x < 0.8 && cursorpos.y>0.15 && cursorpos.y < 0.35)
	{
		cout << "µà¿À ¼±ÅÃ" << endl;

		return DUO;
	}
	return DUO;
	
}

SceneState CSceneManager::CheckMapButton(const POINT & pos)
{
	XMFLOAT2 cursorpos{ 2.0f * (static_cast<float>(pos.x) / static_cast<float>(FRAME_BUFFER_WIDTH)) - 1.0f
					, -(2.0f * (static_cast<float>(pos.y) / static_cast<float>(FRAME_BUFFER_HEIGHT)) - 1.0f) };


	if (cursorpos.x > -0.8 && cursorpos.x < -0.3 && cursorpos.y>0.1 && cursorpos.y < 0.35)
	{
		cout << "PLAYGROUNDMAP¸Ê ¼±ÅÃ" << endl;
		return PLAYGROUNDMAP;
	}
	else if (cursorpos.x > -0.8 && cursorpos.x < -0.3 && cursorpos.y>-0.75 && cursorpos.y < -0.2)
	{
		cout << "CONCERTMAP¸Ê ¼±ÅÃ" << endl;

		return CONCERTMAP;
	}
	return PLAYGROUNDMAP;

}

CSceneManager::CSceneManager()
{
	m_pTempScene = NULL;// std::make_shared<CTempScene>();
	
	
}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::SetScene(SceneState state)
{
	/*if (m_pTempScene != NULL)
	{

		switch (state)
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
		}

	}*/
	/*if (m_pTempScene)
	{
		delete m_pTempScene;
		m_pTempScene = NULL;
	}
	switch (state)
	{
	case MENUSCENE:
		m_pTempScene = new CTempLoadScene;
		m_SceneType = MENUSCENE;

		break;
	case INGAME:
		m_pTempScene = new ;
		m_SceneType = INGAME;

		break;
	case GAMEOVER:
		m_pTempScene = new CTempGameOverScene;
		m_SceneType = GAMEOVER;

		break;

	}*/
	m_SceneType = state;
}
