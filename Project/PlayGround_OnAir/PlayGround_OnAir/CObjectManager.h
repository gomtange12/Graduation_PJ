#pragma once

class CGameObject;
class CLoadedModelInfo;
class CObjectManager : public CSingleTonBase<CObjectManager>
{
	std::map<OBJTYPE, std::vector<CGameObject*>> m_ObjMap;
public:
	std::vector<CGameObject*> ObjList;
public:
	std::map<E_CHARACTERTYPE, CLoadedModelInfo*> m_PlayerResourceMap;
	std::map<E_CHARACTERTYPE, std::shared_ptr<CTerrainPlayer>> m_PlayerTempMap;

	CObjectManager();
	~CObjectManager();
public:
	void LoadPlayerResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature   *m_pd3dGraphicsRootSignature);

	void AddGameObject(OBJTYPE objType ,char* fname);
	void SetObjlist(vector<CGameObject*> &Obj);
	std::vector<CGameObject*> GetObjlist() { return ObjList; }
	std::map<OBJTYPE, std::vector<CGameObject*>> GetAllObjList() { return m_ObjMap; }
	CGameObject* FindGameObject(OBJTYPE objType, int idx);
	CLoadedModelInfo* CObjectManager::GetPlayerResource(E_CHARACTERTYPE type);

};

