#pragma once
#include <map>
class CGameObject;
class CObjectManager : public CSingleTonBase<CObjectManager>
{
	std::map<OBJTYPE, std::vector<CGameObject*>> m_ObjMap;
public:
	std::vector<CGameObject*> ObjList;
public:
	CObjectManager();
	~CObjectManager();
public:
	void AddGameObject(CGameObject* obj, OBJTYPE objType);
	void SetObjlist(vector<CGameObject*> &Obj);
	std::vector<CGameObject*> GetObjlist() { return ObjList; }
	CGameObject* FindGameObject(OBJTYPE objType, int idx);

};

