#pragma once
#include "MyData.h"
class CPlayer;
class CTerrainPlayer;



class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
	std::shared_ptr<CTerrainPlayer>							m_pPlayer = nullptr;
	//std::shared_ptr<COtherPlayer>							m_pOtherPlayer = nullptr;

	//std::vector<std::shared_ptr<COtherPlayer>>				m_pOtherPlayer;
	std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>>			m_PlayerMap;
public:
	vector<CTerrainPlayer*>									m_vecPlayerList;
	CPlayerManager();
	~CPlayerManager();
	void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	void MakeOtherPlayers(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);

	std::shared_ptr<CTerrainPlayer> GetPlayer() { return m_pPlayer; }
	//std::shared_ptr<COtherPlayer> GetOtherPlayer() { return m_pOtherPlayer; }

	//std::shared_ptr<CTerrainPlayer> SetPlayer() { return m_pPlayer; }
	int m_MaxPlayerNum{ 8 };
	void AddPlayer(CTerrainPlayer* playerObj, E_PLAYERTYPE type, E_CHARACTERTYPE charactertype);

	void SetPlayerResource();
	//std::map<E_PLAYERTYPE, vector<CTerrainPlayer*>> GetPlayerMap() { return m_PlayerMap; }
	//std::vector<CTerrainPlayer*> GetPlayerList() { return m_vecPlayerList; }


	
};

