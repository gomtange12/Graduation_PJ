#pragma once
#include <memory.h>
#include "MyData.h"
class CPlayer;
class CTerrainPlayer;
class COtherPlayers;

class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
	std::shared_ptr<CTerrainPlayer>							m_pPlayer = nullptr;
	//std::unique_ptr<COtherPlayers>							m_pOtherPlayer = nullptr;
	//COtherPlayers* m_pOtherPlayer;
	COtherPlayers* m_pOtherPlayer{ nullptr };
	std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>>			m_PlayerMap;
	std::vector<COtherPlayers*>								m_pOtherPlayerMap;
public:
	std::vector<COtherPlayers*>		GetOtherPlayerMap() { return m_pOtherPlayerMap; }
	vector<CTerrainPlayer*>									m_vecPlayerList;
	//void 
	CPlayerManager();
	~CPlayerManager();
	//CLoadedModelInfo* MakePlayerResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext, AVATARNumber avartarNum);
	
	void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	void MakeOtherPlayers(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);

	std::shared_ptr<CTerrainPlayer> GetPlayer() { return m_pPlayer; }
	COtherPlayers* GetOtherPlayer() { return m_pOtherPlayer; }
	//COtherPlayers
	//std::shared_ptr<CTerrainPlayer> SetPlayer() { return m_pPlayer; }
	int m_MaxPlayerNum{ 8 };
	void AddPlayer(CTerrainPlayer* playerObj, E_PLAYERTYPE type, E_CHARACTERTYPE charactertype);

	void SetPlayerResource();
	//std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>> GetPlayerMap() { return m_PlayerMap; }
	//std::vector<CTerrainPlayer*> GetPlayerList() { return m_vecPlayerList; }


	
};

