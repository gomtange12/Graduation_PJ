#include "stdafx.h"
#include <array>

#include "GameFramework.h"
#include "CPlayerManager.h"
#include "Player.h"
#include "CNetWork.h"
E_CHARACTERTYPE CPlayerManager::CheckSceneCharacter(const POINT& pos)
{
	//int size{ 50 };

	XMFLOAT2 cursorpos{  2.0f * (static_cast<float>(pos.x) / static_cast<float>(FRAME_BUFFER_WIDTH)) - 1.0f
					, -(2.0f * (static_cast<float>(pos.y) / static_cast<float>(FRAME_BUFFER_HEIGHT)) - 1.0f) };

	std::cout << "변환후cursorPos: " << cursorpos.x << ", " << cursorpos.y << endl;

	if (cursorpos.x > 0.30 && cursorpos.x < 0.50 && cursorpos.y>-0.15 && cursorpos.y < 0.1)
	{
		cout << "드럼 선택" << endl;
		return DRUM;
	}
	else if (cursorpos.x > 0.59 && cursorpos.x < 0.75 && cursorpos.y>-0.15 && cursorpos.y < 0.1)
	{
		cout << "키보드 선택" << endl;

		return KEYBOARD;
	}
	else if (cursorpos.x > 0.30 && cursorpos.x < 0.50 && cursorpos.y>-0.47 && cursorpos.y < -0.15)
	{
		cout << "베이스 선택" << endl;

		return  BASS;
	}
	else if (cursorpos.x > 0.59 && cursorpos.x < 0.75 && cursorpos.y>-0.47 && cursorpos.y < -0.15)
	{
		cout << "보컬 선택" << endl;
		return VOCAL;
	}
	else if (cursorpos.x > 0.45 && cursorpos.x < 0.65 && cursorpos.y>-0.8 && cursorpos.y < -0.55)
	{
		cout << "기타 선택" << endl;
		return GUITAR;
	}
	
}
CPlayerManager::CPlayerManager()
{
	m_pPlayer = nullptr;

}

void CPlayerManager::SetOtherModelResource(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pEnemyGuitarModel[i] = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);
		m_pEnemyKeyboardModel[i] = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/KeyT.bin", NULL, true);
		m_pEnemyDrumModel[i] = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/DrumTest.bin", NULL, true);;
		m_pEnemyVocalModel[i] = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/basstest.bin", NULL, true);;
		m_pEnemyBassModel[i] = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/MicTest.bin", NULL, true);
	}


	m_pTeamGuitarModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);
	m_pTeamBassModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/KeyT.bin", NULL, true);
	m_pTeamKeyboardModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/DrumTest.bin", NULL, true);;
	m_pTeamDrumModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/basstest.bin", NULL, true);;
	m_pTeamVocalModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/MicTest.bin", NULL, true);

}
void CPlayerManager::SetOtherPlayerResourceFromPool(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pEnemyGuitarPlayer[i] = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext);
		m_pEnemyKeyboardPlayer[i] = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
		m_pEnemyDrumPlayer[i] = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext);
		m_pEnemyVocalPlayer[i] = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext);
		m_pEnemyBassPlayer[i] = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext);
	}

	m_pTeamGuitarPlayer = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext);
	m_pTeamBassPlayer = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
	m_pTeamKeyboardPlayer = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext);
	m_pTeamDrumPlayer = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext);
	m_pTeamVocalPlayer = new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext);


}
COtherPlayers* CPlayerManager::GetOtherPlayerResourceFromPool(int num, E_CHARACTERTYPE type, bool isTeam)
{
	if (type == GUITAR)
	{
		if (!isTeam)
		{
			return m_pEnemyGuitarPlayer[num];
		}
		return m_pTeamGuitarPlayer;
	}
	else if (type == BASS)
	{
		if (!isTeam)
		{
			return m_pEnemyBassPlayer[num];
		}
		return m_pTeamBassPlayer;
	}
	else if (type == KEYBOARD)
	{
		if (!isTeam)
		{
			return m_pEnemyKeyboardPlayer[num];
		}
		return m_pTeamKeyboardPlayer;
	}
	else if (type == DRUM)
	{
		if (!isTeam)
		{
			return m_pEnemyDrumPlayer[num];
		}
		return m_pTeamDrumPlayer;
	}
	else if (type == VOCAL)
	{
		if (!isTeam)
		{
			return m_pEnemyVocalPlayer[num];
		}
		return m_pTeamVocalPlayer;
	}
}

CLoadedModelInfo* CPlayerManager::GetOtherModelResourceFromPool(int num, E_CHARACTERTYPE type, bool isTeam)
{
	if (type == GUITAR)
	{
		if (!isTeam)
		{
			return m_pEnemyGuitarModel[num];
		}
		return m_pTeamGuitarModel;
	}
	else if (type == BASS)
	{
		if (!isTeam)
		{
			return m_pEnemyBassModel[num];
		}
		return m_pTeamBassModel;
	}
	else if (type == KEYBOARD)
	{
		if (!isTeam)
		{
			return m_pEnemyKeyboardModel[num];
		}
		return m_pTeamKeyboardModel;
	}
	else if (type == DRUM)
	{
		if (!isTeam)
		{
			return m_pEnemyDrumModel[num];
		}
		return m_pTeamDrumModel;
	}
	else if (type == VOCAL)
	{
		if (!isTeam)
		{
			return m_pEnemyVocalModel[num];
		}
		return m_pTeamVocalModel;
	}
}
CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::ChangePlayer(E_CHARACTERTYPE type, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	CLoadedModelInfo*         m_pGuitarModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);
	CLoadedModelInfo*         m_pKeyBoardModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/KeyT.bin", NULL, true);
	CLoadedModelInfo*         m_pDrumModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/DrumTest.bin", NULL, true);;
	CLoadedModelInfo*         m_pBassModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/basstest.bin", NULL, true);;
	CLoadedModelInfo*         m_pVocalModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/MicTest.bin", NULL, true);


	if (type == GUITAR)
	{
		m_pPlayer = m_pGuitarPlayer;// make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
		m_pPlayer->SetChild(m_pGuitarModel->m_pModelRootObject);
		m_pPlayer->SetScale(XMFLOAT3(60, 60, 60));
		m_pPlayer->SetMesh(m_pGuitarModel->m_pModelRootObject->m_pMesh);
		m_pPlayer->SetCharacterType(GUITAR);
	}

	else if (type == KEYBOARD)
	{
		m_pPlayer = m_pKeyboardPlayer;// make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
		m_pPlayer->SetChild(m_pKeyBoardModel->m_pModelRootObject);
		m_pPlayer->SetScale(XMFLOAT3(60, 60, 60));
		m_pPlayer->SetMesh(m_pKeyBoardModel->m_pModelRootObject->m_pMesh);
		m_pPlayer->SetCharacterType(KEYBOARD);

	}

	else if (type == DRUM)
	{

		m_pPlayer = m_pDrumPlayer;// make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
		m_pPlayer->SetChild(m_pDrumModel->m_pModelRootObject);
		m_pPlayer->SetScale(XMFLOAT3(60, 60, 60));
		m_pPlayer->SetMesh(m_pDrumModel->m_pModelRootObject->m_pMesh);
		m_pPlayer->SetCharacterType(DRUM);

	}
	else if (type == VOCAL)
	{
		m_pPlayer = m_pVocalPlayer;// make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
		m_pPlayer->SetChild(m_pVocalModel->m_pModelRootObject);
		m_pPlayer->SetScale(XMFLOAT3(60, 60, 60));
		m_pPlayer->SetMesh(m_pVocalModel->m_pModelRootObject->m_pMesh);
		m_pPlayer->SetCharacterType(VOCAL);

	}
	else if (type == BASS)
	{
		m_pPlayer = m_pBassPlayer;// make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
		m_pPlayer->SetChild(m_pBassModel->m_pModelRootObject);
		m_pPlayer->SetScale(XMFLOAT3(60, 60, 60));
		m_pPlayer->SetMesh(m_pBassModel->m_pModelRootObject->m_pMesh);
		m_pPlayer->SetCharacterType(BASS);

	}


}
void CPlayerManager::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	//첫번째 플레이어
	m_pPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
	m_pOtherPlayer = std::make_shared<COtherPlayers>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext);
	//플레이어맵 
	m_pKeyboardPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext);
	m_pBassPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext);
	m_pDrumPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext);
	m_pGuitarPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext);
	m_pVocalPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext);

	m_SharedPlayerMap.emplace(KEYBOARD, m_pKeyboardPlayer);
	m_SharedPlayerMap.emplace(BASS, m_pBassPlayer);
	m_SharedPlayerMap.emplace(DRUM, m_pDrumPlayer);
	m_SharedPlayerMap.emplace(GUITAR, m_pGuitarPlayer);
	m_SharedPlayerMap.emplace(VOCAL, m_pVocalPlayer);


	//if (CNETWORK->GetInstance()->)

	MakeOtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);
}

void CPlayerManager::MakeOtherPlayers(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	//여길 고쳐야함. 
   //m_pOtherPlayer = std::make_shared<CTerrainPlayer>(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);
   //m_vecPlayerList.emplace_back(new CTerrainPlayer);
   //AddPlayer()
	{
		m_vecPlayerList.reserve(m_MaxPlayerNum);
		m_pOtherPlayerMap.reserve(m_MaxPlayerNum);
		m_pTeamPlayerMap.reserve(m_MaxPlayerNum);
		m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));


		m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));


		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
	
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//
		////m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));

		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
		//m_pOtherPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
	
		////팀
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, GUITAR, pContext));

		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, VOCAL, pContext));

		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, BASS, pContext));


		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, DRUM, pContext));
	
		//
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
		//m_pTeamPlayerMap.emplace_back(new COtherPlayers(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, KEYBOARD, pContext));
	

	


		//default_random_engine dre;
		//uniform_int_distribution<> otherUid(200, 600);
		//uniform_int_distribution<> teamUid(2300, 2800);
		//uniform_int_distribution<> zUid(1600, 1800);


		m_pTeamPlayerMap[0]->SetPosition(XMFLOAT3(12750, 10, 1745));
		m_pTeamPlayerMap[1]->SetPosition(XMFLOAT3(12750, 10, 1835));
		//m_pTeamPlayerMap[1]->SetPosition(XMFLOAT3(12750, 10, 1835));
		//m_pTeamPlayerMap[2]->SetPosition(XMFLOAT3(2560, 10, 1835));
		//m_pTeamPlayerMap[2]->SetPosition(XMFLOAT3(12560, 10, 1835));


		m_pOtherPlayerMap[0]->SetPosition(XMFLOAT3(1600, 10, 1745));
		m_pOtherPlayerMap[1]->SetPosition(XMFLOAT3(1350, 10, 1745));
		//m_pOtherPlayerMap[2]->SetPosition(XMFLOAT3(600, 10, 1835));
		//m_pOtherPlayerMap[3]->SetPosition(XMFLOAT3(350, 10, 1835));
		//m_pOtherPlayerMap[2]->SetPosition(XMFLOAT3(11600, 10, 1835));
		//m_pOtherPlayerMap[3]->SetPosition(XMFLOAT3(11350, 10, 1835));


		for (auto&& p : m_pOtherPlayerMap) //팀원이 아닌경우
		{
			p->SetScale(XMFLOAT3(60.0f, 60, 60));
			p->Rotate(0, 180, 0);
		}

		for (auto&& p : m_pTeamPlayerMap) //팀원인경우
		{
			p->SetScale(XMFLOAT3(60.0f, 60, 60));
			p->Rotate(0, 180, 0);
		}
	}
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

void CPlayerManager::SetOtherPlayerResource(int num, E_CHARACTERTYPE type, bool isTeam)
{
	
}
