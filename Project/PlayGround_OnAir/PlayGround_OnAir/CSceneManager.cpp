#include "stdafx.h"

#include "CSceneManager.h"
#include "CTempScene.h"
#include "Scene.h"
#include "GameFramework.h"
void CSceneManager::SetColock()
{
	if (m_Gameclock > 0) m_Gameclock -= 1;

	m_hunSec = m_Gameclock / 100;
	m_tenSec = (m_Gameclock % 100) / 10;
	m_oneSec = m_Gameclock % 10;
}
ModNumber CSceneManager::CheckModeButton(const POINT & pos)
{
	XMFLOAT2 cursorpos{ 2.0f * (static_cast<float>(pos.x) / static_cast<float>(FRAME_BUFFER_WIDTH)) - 1.0f
					, -(2.0f * (static_cast<float>(pos.y) / static_cast<float>(FRAME_BUFFER_HEIGHT)) - 1.0f) };

	if (cursorpos.x > 0.25 && cursorpos.x < 0.55 && cursorpos.y>0.15 && cursorpos.y < 0.35)
	{
		//cout << "¼Ö·Î ¼±ÅÃ" << endl;
		m_isModeSelected = true;

		return SOLO;
	}
	else if (cursorpos.x > 0.55 && cursorpos.x < 0.8 && cursorpos.y>0.15 && cursorpos.y < 0.35)
	{
		//cout << "µà¿À ¼±ÅÃ" << endl;
		m_isModeSelected = true;

		return SQUAD;
	}
	return MODNONE;
	
}

SceneState CSceneManager::CheckMapButton(const POINT & pos)
{
	XMFLOAT2 cursorpos{ 2.0f * (static_cast<float>(pos.x) / static_cast<float>(FRAME_BUFFER_WIDTH)) - 1.0f
					, -(2.0f * (static_cast<float>(pos.y) / static_cast<float>(FRAME_BUFFER_HEIGHT)) - 1.0f) };


	if (cursorpos.x > -0.8 && cursorpos.x < -0.3 && cursorpos.y>0.1 && cursorpos.y < 0.35)
	{
		//cout << "PLAYGROUNDMAP¸Ê ¼±ÅÃ" << endl;
		m_isMapSelected = true;
		return PLAYGROUNDMAP;
	}
	else if (cursorpos.x > -0.8 && cursorpos.x < -0.3 && cursorpos.y>-0.75 && cursorpos.y < -0.2)
	{
		//cout << "CONCERTMAP¸Ê ¼±ÅÃ" << endl;
		m_isMapSelected = true;

		return CONCERTMAP;
	}
	return NONE;

}

CSceneManager::CSceneManager()
{
	m_pTempScene = NULL;// std::make_shared<CTempScene>();
	m_Gameclock = 300;
	
}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::SetScene(SceneState state)
{
	m_SceneType = state;
	if(state == MENUSCENE)
		PlaySound(L"Sound/MENU.wav", NULL, SND_FILENAME | SND_ASYNC);
	if(state == PLAYGROUNDMAP)
		PlaySound(L"Sound/INGAME.wav", NULL,  SND_ASYNC | SND_NOSTOP );

		//	SCENEMANAGER->SetMunuMusicOn(false);
		
}
