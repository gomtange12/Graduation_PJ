#include "stdafx.h"
#include "CObjectManager.h"
#include "Object.h"

CObjectManager::CObjectManager()
{
}


CObjectManager::~CObjectManager()
{
}

void CObjectManager::AddGameObject(CGameObject * obj, OBJTYPE objType)
{
	//todo ���ӿ�����Ʈ ��ӹ��� Ŭ������ �Ѱܹް�. �Ѱܹ������ �����ϰ�

	//�� Ÿ�Ը������ ���� Ȯ���ϰ�
	//�װ� ���ٸ� �׋� �μ�Ʈ�� ����. 


	//�ֳ� ���� Ȯ��
	auto iter = m_ObjMap.find(objType);
	if (iter != m_ObjMap.end()) //������ ����Ʈ�� ����
	{
		//obj->SetPosition(xPos, yPos);
		//obj->SetSize(size);
		//obj->SetType(objType);
		//obj->SetHp(20);//temp
		m_ObjMap[objType].push_back(obj);
		//m_ObjMap[objType].begin()
	}
	else //������ ����Ʈ�� ���� ����
	{
		//obj->SetPos(xPos, yPos);
		//obj->SetSize(size);
		//obj->SetType(objType);
		//obj->SetHp(20);//temp
		ObjList.push_back(obj);
		m_ObjMap.insert(std::make_pair(objType, ObjList));

	}

	//������ �� ������Ʈ����Ʈ�� ����. m_ObjMap[objType].pushback(obj)

}

void CObjectManager::SetObjlist(vector<CGameObject*>& Obj)
{
	Obj = ObjList;

}

CGameObject * CObjectManager::FindGameObject(OBJTYPE objType, int idx)
{
	//_ObjMap.find(obj)
	/*for (unsigned int i = 0; i < m_ObjMap[objType].size(); ++i)
	{
		if (m_ObjMap[objType][i]->GetMyIdx() == idx)
			return m_ObjMap[objType][i];
	}*/
	return NULL;

}
