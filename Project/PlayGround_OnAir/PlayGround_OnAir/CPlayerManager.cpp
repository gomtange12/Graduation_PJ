#include "stdafx.h"
#include "GameFramework.h"
#include "CPlayerManager.h"
#include "Player.h"
CPlayerManager::CPlayerManager()
{
	

}


CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature,pContext);
	//4m_pPlayer->SETPO

	int i = 0;
}


