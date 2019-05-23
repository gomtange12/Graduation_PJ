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
	void AddGameObject(OBJTYPE objType ,char* fname, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	void SetObjlist(vector<CGameObject*> &Obj);
	std::vector<CGameObject*> GetObjlist() { return ObjList; }
	std::map<OBJTYPE, std::vector<CGameObject*>> GetAllObjList() { return m_ObjMap; }
	CGameObject* FindGameObject(OBJTYPE objType, int idx);
};

