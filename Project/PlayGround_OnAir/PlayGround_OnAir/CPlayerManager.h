#pragma once
class CTerrainPlayer;
//class COtherPlayer;
class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
	std::shared_ptr<CTerrainPlayer>							m_pPlayer;

	//std::vector<std::shared_ptr<COtherPlayer>>				m_pOtherPlayer;

public:
	CPlayerManager();
	~CPlayerManager();
	void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	std::shared_ptr<CTerrainPlayer> GetPlayer() { return m_pPlayer; }
	//std::shared_ptr<CTerrainPlayer> SetPlayer() { return m_pPlayer; }


};

