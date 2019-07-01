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
	//todo ���ӿ�����Ʈ ��ӹ��� Ŭ������ �Ѱܹް�. �Ѱܹ������ �����ϰ�

	//�� Ÿ�Ը������ ���� Ȯ���ϰ�
	//�װ� ���ٸ� �׋� �μ�Ʈ�� ����. 

//�ֳ� ���� Ȯ��
	//CLoadedModelInfo* obj = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, fname, NULL, false);
	//auto iter = m_ObjMap.find(objType);
	//if (iter != m_ObjMap.end()) //������ ����Ʈ�� ����
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
	//else //������ ����Ʈ�� ���� ����
	//{
	//	obj->SetPosition(XMFLOAT3(0, 0, 0));

	//	obj->SetObjType(objType);
	//	obj->SetHp(20);//temp
	//	ObjList.emplace_back(obj);
	//	m_ObjMap.insert(std::make_pair(objType, ObjList));

	//}
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
void CObjectManager::LoadPlayerResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature   *m_pd3dGraphicsRootSignature)
{
	CLoadedModelInfo* guitarResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);
	CLoadedModelInfo* keyBoardResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/KeyT.bin", NULL, true);
	CLoadedModelInfo* DrumResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/DrumTest.bin", NULL, true);
	CLoadedModelInfo* vocalResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/MicTest.bin", NULL, true);
	CLoadedModelInfo* bassResource = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/basstest.bin", NULL, true);

	m_PlayerResourceMap.insert(std::make_pair(GUITAR, guitarResource));
	m_PlayerResourceMap.insert(std::make_pair(KEYBOARD, keyBoardResource));
	m_PlayerResourceMap.insert(std::make_pair(BASS, bassResource));
	m_PlayerResourceMap.insert(std::make_pair(DRUM, DrumResource));
	m_PlayerResourceMap.insert(std::make_pair(VOCAL, vocalResource));

}

CLoadedModelInfo* CObjectManager::GetPlayerResource(E_CHARACTERTYPE type)
{
	auto iter = m_PlayerResourceMap.find(type);

	if (iter != m_PlayerResourceMap.end())
	{
		return (*iter).second;
		//m_ObjMap[objType].begin();

	}
	else //����ó��
	{
		return m_PlayerResourceMap.begin()->second;
	}

}