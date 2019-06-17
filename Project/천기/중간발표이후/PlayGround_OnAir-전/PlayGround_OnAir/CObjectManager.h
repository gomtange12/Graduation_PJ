#pragma once

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
	void AddGameObject(OBJTYPE objType ,char* fname);
	void SetObjlist(vector<CGameObject*> &Obj);
	std::vector<CGameObject*> GetObjlist() { return ObjList; }
	std::map<OBJTYPE, std::vector<CGameObject*>> GetAllObjList() { return m_ObjMap; }
	CGameObject* FindGameObject(OBJTYPE objType, int idx);
};

