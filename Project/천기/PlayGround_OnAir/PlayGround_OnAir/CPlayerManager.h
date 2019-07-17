#pragma once
#include "stdafx.h"
#include "MyData.h"
class CPlayer;
class CTerrainPlayer;

class CLoadedModelInfo;
class COtherPlayers;
class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
	std::shared_ptr<CTerrainPlayer>							m_pPlayer = nullptr;
	std::shared_ptr<COtherPlayers>							m_pOtherPlayer = nullptr;

	//std::vector<std::shared_ptr<COtherPlayer>>				m_pOtherPlayer;
	std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>>			m_PlayerMap;
	COtherPlayers*                     m_pEnemyGuitarPlayer[3];
	COtherPlayers*                     m_pEnemyBassPlayer[3];
	COtherPlayers*                     m_pEnemyKeyboardPlayer[3];
	COtherPlayers*                     m_pEnemyDrumPlayer[3];
	COtherPlayers*                     m_pEnemyVocalPlayer[3];

	COtherPlayers*                     m_pTeamGuitarPlayer[3];
	COtherPlayers*                     m_pTeamBassPlayer[3];
	COtherPlayers*                     m_pTeamKeyboardPlayer[3];
	COtherPlayers*                     m_pTeamDrumPlayer[3];
	COtherPlayers*                     m_pTeamVocalPlayer[3];

	CLoadedModelInfo*                     m_pEnemyGuitarModel[3];
	CLoadedModelInfo*                     m_pEnemyBassModel[3];
	CLoadedModelInfo*                     m_pEnemyKeyboardModel[3];
	CLoadedModelInfo*                     m_pEnemyDrumModel[3];
	CLoadedModelInfo*                     m_pEnemyVocalModel[3];

	CLoadedModelInfo*                     m_pTeamGuitarModel;
	CLoadedModelInfo*                     m_pTeamBassModel;
	CLoadedModelInfo*                     m_pTeamKeyboardModel;
	CLoadedModelInfo*                     m_pTeamDrumModel;
	CLoadedModelInfo*                     m_pTeamVocalModel;
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

	int m_MaxPlayerNum{ 8 };
	void AddPlayer(CTerrainPlayer* playerObj, E_PLAYERTYPE type, E_CHARACTERTYPE charactertype);

	void SetOtherPlayerResource(int num, E_CHARACTERTYPE type, bool isTeam);

	COtherPlayers* GetOtherPlayerResourceFromPool(int num, E_CHARACTERTYPE type, bool isTeam);
	CLoadedModelInfo* GetOtherModelResourceFromPool(int num, E_CHARACTERTYPE type, bool isTeam);
	void SetOtherModelResource(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext);
	void SetOtherPlayerResourceFromPool(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext);


	
};

