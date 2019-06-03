#include "stdafx.h"

#include "GameFramework.h"
#include "CPlayerManager.h"
#include "Player.h"
#include "CNetWork.h"
CPlayerManager::CPlayerManager()
{
	m_pPlayer = nullptr;

}


CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	//ù��° �÷��̾�
	m_pPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature,pContext);
	m_pOtherPlayer = std::make_shared<COtherPlayers>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);
	///m_pSECOND = std::make_shared<COtherPlayers>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);


	//4m_pPlayer->SETPO
	//m_vecPlayerList.reserve(m_MaxPlayerNum);
	m_pOtherPlayerMap.reserve(8);
	//std::shared_ptr<std::vector<uint8_t> > mSharedPtr(new std::vector<uint8_t>());
	//for (int i = 0; i < m_MaxPlayerNum - 1; ++i)
	//m_pOtherPlayerMap.emplace_back(shared_ptr<COtherPlayers>(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext)));
	//MakeOtherPlayers(pd3dDevice,pd3dCommandList,pd3dGraphicsRootSignature, pContext);

	m_pOtherPlayerMap.emplace_back(m_pOtherPlayer);
	m_pOtherPlayerMap.emplace_back(m_pOtherPlayer);
	m_pOtherPlayerMap.emplace_back(m_pOtherPlayer);

	cout <<"�ƴ� �÷��̾� �� ũ��  "<< m_pOtherPlayerMap.size() << endl;
	//1111m_pOtherPlayerMap.emplace_back(m_pSECOND);

}

void CPlayerManager::MakeOtherPlayers(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	//���� ���ľ���. 
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
	//0418 �÷��̾ ���� �����ӿ�ũ�ܿ� �����ؼ� �װ� �ڸ��� �ű�� �����ؼ�
	// �� �� �ְ� ��������
	//auto iter = m_PlayerMap.find(objType);
	//if (iter != m_PlayerMap.end()) //������ ����Ʈ�� ����
	//{
	//	//obj->SetPosition(xmf, yPos);
	//	//obj->SetSize(size);
	//	//obj->SetType(objType);
	//	//obj->SetHp(20);//temp
	//	playerObj->SetPosition(XMFLOAT3(100, 150, 200));
	//	m_PlayerMap[objType].push_back(playerObj);
	//	m_PlayerMap[objType].begin();
	//}
	//else //������ ����Ʈ�� ���� ����
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
