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
	//todo ���ӿ�����Ʈ ��ӹ��� Ŭ������ �Ѱܹް�. �Ѱܹ������ �����ϰ�

	//�� Ÿ�Ը������ ���� Ȯ���ϰ�
	//�װ� ���ٸ� �׋� �μ�Ʈ�� ����. 

//�ֳ� ���� Ȯ��
	CLoadedModelInfo* obj = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, fname, nullptr, false);
	
	CGameObject* gameObject = new CGameObject();

	auto iter = m_ObjMap.find(objType);
	if (iter != m_ObjMap.end()) //������ ����Ʈ�� ����
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
	else //������ ����Ʈ�� ���� ����
	{
		obj->SetPosition(XMFLOAT3(0, 0, 0));

		obj->SetObjType(objType);
		obj->SetHp(20);//temp
		ObjList.emplace_back(obj);
		m_ObjMap.insert(std::make_pair(objType, ObjList));

	}
	//������ �� ������Ʈ����Ʈ�� ����. m_ObjMap[objType].pushback(obj)

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
