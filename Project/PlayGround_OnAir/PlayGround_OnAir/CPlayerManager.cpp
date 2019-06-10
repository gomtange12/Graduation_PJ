#include "stdafx.h"
#include "GameFramework.h"
#include "CPlayerManager.h"
#include "Player.h"
#include "COtherPlayer.h"
#include "CNetWork.h"
CPlayerManager::CPlayerManager()
{
	m_pPlayer = nullptr;

}


CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void* pContext)
{
	//첫번째 플레이어
	m_pPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext);
	//m_pOtherPlayer = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext);
	///m_pSECOND = std::make_shared<COtherPlayers>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);


	//4m_pPlayer->SETPO
	//m_vecPlayerList.reserve(m_MaxPlayerNum);
	m_pOtherPlayerMap.reserve(8);

	// 생성자에 리소스
	//m_pOtherPlayerMap.emplace_back(std::make_shared<COtherPlayers>( pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
	//cout << "sizeofMap: " <<m_pOtherPlayerMap.size() << std::endl;
	
	//같은 otherPlayer 여러개 생성
	m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
	m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
	m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));

	cout <<"아더 플레이어 맵 크기  "<< m_pOtherPlayerMap.size() << endl;
	//1111m_pOtherPlayerMap.emplace_back(m_pSECOND);

}

void CPlayerManager::MakeOtherPlayers(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	//여길 고쳐야함. 
	//m_pOtherPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);
	//AddPlayer()
	
	//m_vecPlayerList.emplace_back(new CTerrainPlayer);

	
	//AddPlayer()
}

//CLoadedModelInfo* CPlayerManager::MakePlayerResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext, AVATARNumber avartarNum)
//{
//	switch (avartarNum)
//	{
//	case A:
//		CLoadedModelInfo *pKeytarPlayerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/KeytarTest.bin", NULL, true);
//		return pKeytarPlayerModel;
//		break;
//	case B:
//		CLoadedModelInfo *pGuitarPlayerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);
//		return pGuitarPlayerModel;
//		break;
//	}
//
//
//}
void CPlayerManager::AddPlayer(CTerrainPlayer* playerObj, E_PLAYERTYPE objType, E_CHARACTERTYPE charactertype)
{
	//0418 플레이어가 지금 프레임워크단에 존재해서 그걸 자리를 옮기든 뺴든해서
	// 고를 수 있게 만들어야함
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

void CPlayerManager::SetPlayerResource()
{

}
