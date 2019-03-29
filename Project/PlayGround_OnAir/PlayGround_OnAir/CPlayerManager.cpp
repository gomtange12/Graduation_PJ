#include "stdafx.h"

#include "GameFramework.h"
#include "CPlayerManager.h"
#include "Player.h"
CPlayerManager::CPlayerManager()
{
	m_pPlayer = nullptr;

}


CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature,pContext);
	//m_pOtherPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);

	//4m_pPlayer->SETPO
	//m_vecPlayerList.reserve(m_MaxPlayerNum);
	//m_vecPlayerList.emplace_back(m_pOtherPlayer);

	//AddPlayer(m_pOtherPlayer, ENEMY, BASS);
	
}

void CPlayerManager::AddPlayer(shared_ptr<CTerrainPlayer> playerObj, E_PLAYERTYPE objType, E_CHARACTERTYPE charactertype)
{

	//auto iter = m_PlayerMap.find(objType);
	//if (iter != m_PlayerMap.end()) //있으면 리스트에 넣음
	//{
	//	//obj->SetPosition(xmf, yPos);
	//	//obj->SetSize(size);
	//	//obj->SetType(objType);
	//	//obj->SetHp(20);//temp
	//	playerObj->SetPosition(XMFLOAT3(100, 150, 200));
	//	m_PlayerMap[objType].push_back(playerObj);
	//	m_PlayerMap[objType].begin();
	//}
	//else //없으면 리스트를 만들어서 넣음
	//{
	//	//obj->SetPos(xPos, yPos);
	//	//obj->SetSize(size);
	//	//obj->SetType(objType);
	//	//obj->SetHp(20);
	//	m_vecPlayerList.push_back(playerObj);
	//	m_PlayerMap.insert(std::make_pair(objType, m_vecPlayerList));

	//}
}
