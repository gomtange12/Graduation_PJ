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
	int m_Gameclock{ 300 };

	int m_oneSec{ 0 };
	int m_tenSec{ 0 };
	int m_hunSec{ 0 };
	bool m_playMenuMusic = true;
	bool m_playIngameMusic = true;

public:
	void SetMunuMusicOn(bool on) { m_playMenuMusic = on; }
	void SetIngameMusicOn(bool on) { m_playIngameMusic = on; }

	bool GetMenuMusicOn() { return m_playMenuMusic; }
	bool GetIngameMusicOn() { return m_playIngameMusic; }

	int  GetOneSec() const { return m_oneSec; }
	int  GetTenSec() const { return m_tenSec; }
	int  GetHunSec() const { return m_hunSec; }

	void recvDonation() { m_Gameclock += 10; }
	int  GetGameClock() { return m_Gameclock; }
	bool GetSelectedMap() { return m_isMapSelected; }
	bool GetSelectedMode() { return m_isModeSelected; }
	void SetSelectedMode(bool mode) { m_isModeSelected = mode; }
	void SetSelectedMap(bool map) { m_isMapSelected = map; }
	void SetColock();
	ModNumber CheckModeButton(const POINT& pos);
	SceneState CheckMapButton(const POINT& pos);

	std::map<SceneState, CScene*> m_MapList;
	CSceneManager();
	~CSceneManager();
	SceneState GetSceneType() { return m_SceneType; }
	void SetScene(SceneState state);
};

