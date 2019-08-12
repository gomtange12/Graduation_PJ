#include "stdafx.h"
#include "CObjectManager.h"
#include "Object.h"

CObjectManager::CObjectManager()
{
}


CObjectManager::~CObjectManager()
{
}

void CObjectManager::AddGameObject(OBJTYPE objType, char* fname)
{
	//todo 게임오브젝트 상속받은 클래스를 넘겨받게. 넘겨받은대로 생성하게

	//이 타입만들어진 맵을 확인하고
	//그게 없다면 그떄 인서트로 만듬. 

//있나 없나 확인
	//CLoadedModelInfo* obj = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, fname, NULL, false);
	//auto iter = m_ObjMap.find(objType);
	//if (iter != m_ObjMap.end()) //있으면 리스트에 넣음
	//{
	//	obj->SetPosition(XMFLOAT3(0,0,0));
	//	
	//	obj->SetObjType(objType);
	//	obj->SetHp(20);//temp
	//	if (objType == MAPOBJ)
	//	{
	//		
	//	}

	//	m_ObjMap[objType].emplace_back(obj);
	//	m_ObjMap[objType].begin();
	//}
	//else //없으면 리스트를 만들어서 넣음
	//{
	//	obj->SetPosition(XMFLOAT3(0, 0, 0));

	//	obj->SetObjType(objType);
	//	obj->SetHp(20);//temp
	//	ObjList.emplace_back(obj);
	//	m_ObjMap.insert(std::make_pair(objType, ObjList));

	//}
	//있으면 저 오브젝트리스트로 들어가서. m_ObjMap[objType].pushback(obj)

}

void CObjectManager::SetObjlist(vector<CGameObject*>& Obj)
{
	Obj = ObjList;

}

CGameObject* CObjectManager::FindGameObject(OBJTYPE objType, int idx)
{
	//_ObjMap.find(obj)
	/*for (unsigned int i = 0; i < m_ObjMap[objType].size(); ++i)
	{
		if (m_ObjMap[objType][i]->GetMyIdx() == idx)
			return m_ObjMap[objType][i];
	}*/
	return NULL;

}
void CObjectManager::LoadPlayerResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature   *m_pd3dGraphicsRootSignature)
{
	CLoadedModelInfo* guitarResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);
	CLoadedModelInfo* keyBoardResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/KeyT.bin", NULL, true);
	CLoadedModelInfo* DrumResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/DrumTest.bin", NULL, true);
	CLoadedModelInfo* vocalResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/MicTest.bin", NULL, true);
	CLoadedModelInfo* bassResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/bass1-1.bin", NULL, true);

	CLoadedModelInfo* otherGuitarResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/guitarTest2.bin", NULL, true);
	CLoadedModelInfo* otherKeyBoardResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/KeyT2.bin", NULL, true);
	CLoadedModelInfo* otherDrumResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/DrumTest2.bin", NULL, true);
	CLoadedModelInfo* otherVocalResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/MicTest2.bin", NULL, true);
	CLoadedModelInfo* otherBassResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/bass1-1.bin", NULL, true);


	m_PlayerResourceMap.insert(std::make_pair(GUITAR, guitarResource));
	m_PlayerResourceMap.insert(std::make_pair(KEYBOARD, keyBoardResource));
	m_PlayerResourceMap.insert(std::make_pair(BASS, bassResource));
	m_PlayerResourceMap.insert(std::make_pair(DRUM, DrumResource));
	m_PlayerResourceMap.insert(std::make_pair(VOCAL, vocalResource));

	m_OtherPlayerResourceMap.insert(std::make_pair(GUITAR, otherGuitarResource));
	m_OtherPlayerResourceMap.insert(std::make_pair(KEYBOARD, otherKeyBoardResource));
	m_OtherPlayerResourceMap.insert(std::make_pair(BASS, otherBassResource));
	m_OtherPlayerResourceMap.insert(std::make_pair(DRUM, otherDrumResource));
	m_OtherPlayerResourceMap.insert(std::make_pair(VOCAL, otherVocalResource));


}

CLoadedModelInfo* CObjectManager::GetPlayerResource(E_CHARACTERTYPE type)
{
	auto iter = m_PlayerResourceMap.find(type);

	if (iter != m_PlayerResourceMap.end())
	{
		return (*iter).second;
		//m_ObjMap[objType].begin();

	}
	else //예외처리
	{
		return m_PlayerResourceMap.begin()->second;
	}

}

CLoadedModelInfo * CObjectManager::GetOhterPlayerResource(E_CHARACTERTYPE type)
{
	auto iter = m_OtherPlayerResourceMap.find(type);

	if (iter != m_OtherPlayerResourceMap.end())
	{
		return (*iter).second;
		//m_ObjMap[objType].begin();

	}
	else //예외처리
	{
		return m_OtherPlayerResourceMap.begin()->second;
	}
}
