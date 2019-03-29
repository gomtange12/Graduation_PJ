#pragma once
#include "MyData.h"
class CPlayer;
class CTerrainPlayer;

//class COtherPlayer;
class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
	std::shared_ptr<CTerrainPlayer>							m_pPlayer = nullptr;
	std::shared_ptr<CTerrainPlayer>							m_pOtherPlayer = nullptr;

	//std::vector<std::shared_ptr<COtherPlayer>>				m_pOtherPlayer;
	std::map<E_PLAYERTYPE, vector<shared_ptr<CTerrainPlayer>>>		m_PlayerMap;
	vector<shared_ptr<CTerrainPlayer>>						m_vecPlayerList;
public:
	CPlayerManager();
	~CPlayerManager();
	void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	std::shared_ptr<CTerrainPlayer> GetPlayer() { return m_pPlayer; }
	//std::shared_ptr<CTerrainPlayer> SetPlayer() { return m_pPlayer; }
	//int m_MaxPlayerNum{ 8 };
	void AddPlayer(shared_ptr<CTerrainPlayer> playerObj, E_PLAYERTYPE type, E_CHARACTERTYPE charactertype);


	//std::map<E_PLAYERTYPE, vector<shared_ptr<CTerrainPlayer>>>& GetPlayerMap() { return m_PlayerMap; }
	//vector<shared_ptr<CTerrainPlayer>>& GetPlayerList() { return m_vecPlayerList; }
};

