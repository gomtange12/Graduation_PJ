#pragma once
#include "stdafx.h"
#include "MyData.h"
class CPlayer;
class CTerrainPlayer;


class COtherPlayers;
class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
	std::shared_ptr<CTerrainPlayer>							m_pPlayer = nullptr;
	std::shared_ptr<COtherPlayers>							m_pOtherPlayer = nullptr;

	//std::vector<std::shared_ptr<COtherPlayer>>				m_pOtherPlayer;
	std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>>			m_PlayerMap;
	//array<E_CHARACTERTYPE, 5> m_character_Array ;
	//array<E_CHARACTERTYPE, 5> m_Othercharacter_Array;
public:
	std::vector<COtherPlayers*>								m_pOtherPlayerMap;
	std::vector<COtherPlayers*>								m_pTeamPlayerMap;
	std::shared_ptr<CTerrainPlayer>							m_pGuitarPlayer = nullptr;
	std::shared_ptr<CTerrainPlayer>							m_pBassPlayer = nullptr;
	std::shared_ptr<CTerrainPlayer>							m_pKeyboardPlayer = nullptr;
	std::shared_ptr<CTerrainPlayer>							m_pDrumPlayer = nullptr;
	std::shared_ptr<CTerrainPlayer>							m_pVocalPlayer = nullptr;
	std::map<E_CHARACTERTYPE, std::shared_ptr<CTerrainPlayer>>			m_SharedPlayerMap;
	void ChangePlayer(E_CHARACTERTYPE type, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);

	E_CHARACTERTYPE m_characterArray[5];
	E_CHARACTERTYPE m_OthercharacterArray[5];
	E_CHARACTERTYPE* GetCharacterArray() { return m_characterArray; }
	E_CHARACTERTYPE* GetOtherCharacterArray() { return 	m_OthercharacterArray; }
	
	E_CHARACTERTYPE CheckSceneCharacter(const POINT& pos);
	void SetCharacterArray(E_CHARACTERTYPE type, int num) { m_characterArray[num] = type; }
	void SetOtherCharacterArray(E_CHARACTERTYPE type, int num) { m_OthercharacterArray[num] = type; }

	std::vector<COtherPlayers*>		GetOtherPlayerMap() { return m_pOtherPlayerMap; }
	std::vector<COtherPlayers*>		GetTeamPlayerMap() { return m_pTeamPlayerMap; }

	vector<CTerrainPlayer*>									m_vecPlayerList;
	//void 
	CPlayerManager();
	~CPlayerManager();
	//CLoadedModelInfo* MakePlayerResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext, AVATARNumber avartarNum);
	
	void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	void MakeOtherPlayers(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);

	std::shared_ptr<CTerrainPlayer> GetPlayer() { return m_pPlayer; }
	std::shared_ptr<COtherPlayers> GetOtherPlayer() { return m_pOtherPlayer; }

	//std::shared_ptr<CTerrainPlayer> SetPlayer() { return m_pPlayer; }
	int m_MaxPlayerNum{ 8 };
	void AddPlayer(CTerrainPlayer* playerObj, E_PLAYERTYPE type, E_CHARACTERTYPE charactertype);

	void SetOtherPlayerResource(int num, E_CHARACTERTYPE type, bool isTeam);

	//std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>> GetPlayerMap() { return m_PlayerMap; }
	//std::vector<CTerrainPlayer*> GetPlayerList() { return m_vecPlayerList; }


	
};

