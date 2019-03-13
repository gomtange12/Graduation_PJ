#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Player.h"
#include "Scene.h"
#define TEMP_MAX_LIGHTS						16 
#define TEMP_POINT_LIGHT						1
#define TEMP_SPOT_LIGHT						2
#define TEMP_DIRECTIONAL_LIGHT				3
class CGameObject;
class CShader;
class CSkyBox;
class CHeightMapTerrain;
class CScene;
struct TEMPLIGHT
{
	XMFLOAT4							m_xmf4Ambient;
	XMFLOAT4							m_xmf4Diffuse;
	XMFLOAT4							m_xmf4Specular;
	XMFLOAT3							m_xmf3Position;
	float 								m_fFalloff;
	XMFLOAT3							m_xmf3Direction;
	float 								m_fTheta; //cos(m_fTheta)
	XMFLOAT3							m_xmf3Attenuation;
	float								m_fPhi; //cos(m_fPhi)
	bool								m_bEnable;
	int									m_nType;
	float								m_fRange;
	float								padding;
};

struct TEMPLIGHTS
{
	TEMPLIGHT							m_pLights[TEMP_MAX_LIGHTS];
	XMFLOAT4							m_xmf4GlobalAmbient;
	int									m_nLights;
};
class CTempScene
{

public:
	CTempScene();
	~CTempScene();
public:
	int									m_nGameObjects = 0;
	CGameObject							**m_ppGameObjects = NULL;
	ID3D12RootSignature					*m_pd3dGraphicsRootSignature = NULL;
	TEMPLIGHT							*m_pLights = NULL;
	int									m_nLights = 0;

	XMFLOAT4							m_xmf4GlobalAmbient;
	int									m_nShaders = 0;
	CShader								**m_ppShaders = NULL;

	CSkyBox								*m_pSkyBox = NULL;
	CHeightMapTerrain					*m_pTerrain = NULL;

	ID3D12Resource						*m_pd3dcbLights = NULL;
	TEMPLIGHTS								*m_pcbMappedLights = NULL;
	
	//필요한 리소스별로 생성하고 추가.
public: 
	//가상함수
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void Initilize() {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) {} //나중에 추상함수로 ㄱ
	virtual void ReleaseShaderVariables();
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
public:
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceViews(ID3D12Device *pd3dDevice, CTexture *pTexture, UINT nRootParameter, bool bAutoIncrement);
	void AnimateObjects(float fTimeElapsed);
	float								m_fElapsedTime = 0.0f;

	ID3D12RootSignature					*m_pd3dTempGraphicsRootSignature = NULL;
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	static ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorStartHandle;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorNextHandle;
	static void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);

public:
	// 부모 씬에서만 한번 돌아야하는것. 모든 리소스를 여기서 선언하고 관리. 
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void BuildDefaultLightsAndMaterials();

	ID3D12RootSignature *CTempScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	void ReleaseUploadBuffers();
	bool ProcessInput(UCHAR *pKeysBuffer);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera = NULL);

	void ReleaseObjects();
	//정적변수들을 어떻게 할 것인가
};

class CTempLoadScene : public CScene
{
public:
	CTempLoadScene();
	~CTempLoadScene();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

};
class CTempMenuScene : public CScene
{
public:
	CTempMenuScene();
	~CTempMenuScene();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) override;

};
class CTempInGameScene : public CScene
{
public:
	CTempInGameScene();
	~CTempInGameScene();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) override;

};
class CTempGameOverScene : public CScene
{
public:
	CTempGameOverScene();
	~CTempGameOverScene();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) override;

};