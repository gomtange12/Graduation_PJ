#include "stdafx.h"
#include "CObjectManager.h"
#include "Object.h"

CObjectManager::CObjectManager()
{
}


CObjectManager::~CObjectManager()
{
}

void CObjectManager::AddGameObject(OBJTYPE objType, char* fname, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL)
{
	//todo 게임오브젝트 상속받은 클래스를 넘겨받게. 넘겨받은대로 생성하게

	//이 타입만들어진 맵을 확인하고
	//그게 없다면 그떄 인서트로 만듬. 

//있나 없나 확인
	CLoadedModelInfo* obj = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, fname, nullptr, false);
	
	CGameObject* gameObject = new CGameObject();

	auto iter = m_ObjMap.find(objType);
	if (iter != m_ObjMap.end()) //있으면 리스트에 넣음
	{
		obj->SetPosition(XMFLOAT3(0,0,0));
		
		obj->SetObjType(objType);
		obj->SetHp(20);//temp
		if (objType == MAPOBJ)
		{
			
		}

		m_ObjMap[objType].emplace_back(obj);
		m_ObjMap[objType].begin();
	}
	else //없으면 리스트를 만들어서 넣음
	{
		obj->SetPosition(XMFLOAT3(0, 0, 0));

		obj->SetObjType(objType);
		obj->SetHp(20);//temp
		ObjList.emplace_back(obj);
		m_ObjMap.insert(std::make_pair(objType, ObjList));

	}
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
