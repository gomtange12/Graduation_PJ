//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "CPlayerManager.h"
#include "CSceneManager.h"
#include "CObjectManager.h"
#include "CBillboardObject.h"
#define objScale 6.47
ID3D12DescriptorHeap *CScene::m_pd3dCbvSrvDescriptorHeap = NULL;

D3D12_CPU_DESCRIPTOR_HANDLE	CScene::m_d3dCbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	CScene::m_d3dCbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	CScene::m_d3dSrvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	CScene::m_d3dSrvGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	CScene::m_d3dCbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	CScene::m_d3dCbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	CScene::m_d3dSrvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	CScene::m_d3dSrvGPUDescriptorNextHandle;

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 9;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	/*m_pLights[0].m_bEnable = true;
	m_pLights[1].m_fRange = 1000.0f;
	m_pLights[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(200.0f, 500.0f, 0.0f);*/

	//중앙라이트
	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = SPOT_LIGHT;
	m_pLights[0].m_fRange = 300.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(-248.0f, 510.0f, 625.0f); //플레이어 위
	m_pLights[0].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[0].m_fFalloff = 8.0f;
	m_pLights[0].m_fPhi = (float)cos(XMConvertToRadians(30.0f));
	m_pLights[0].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 300.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-385.0f, 510.0f, 431.0f); //플레이어 위
	m_pLights[1].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = SPOT_LIGHT;
	m_pLights[2].m_fRange = 300.0f;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[2].m_xmf3Position = XMFLOAT3(-385.0f, 510.0f, -50.0f); //플레이어 위
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[2].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[2].m_fFalloff = 8.0f;
	m_pLights[2].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[2].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 300.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(-180.0f, 510.0f, -260.0f); //플레이어 위
	m_pLights[3].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[4].m_bEnable = true;
	m_pLights[4].m_nType = SPOT_LIGHT;
	m_pLights[4].m_fRange = 300.0f;
	m_pLights[4].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[4].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[4].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[4].m_xmf3Position = XMFLOAT3(620.0f, 510.0f, -260.0f); //플레이어 위
	m_pLights[4].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_pLights[4].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[4].m_fFalloff = 8.0f;
	m_pLights[4].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[4].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[5].m_bEnable = true;
	m_pLights[5].m_nType = SPOT_LIGHT;
	m_pLights[5].m_fRange = 300.0f;
	m_pLights[5].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[5].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[5].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[5].m_xmf3Position = XMFLOAT3(630.0f, 510.0f, 30.0f); //플레이어 위
	m_pLights[5].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[5].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[5].m_fFalloff = 8.0f;
	m_pLights[5].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[5].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[6].m_bEnable = true;
	m_pLights[6].m_nType = SPOT_LIGHT;
	m_pLights[6].m_fRange = 300.0f;
	m_pLights[6].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[6].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[6].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[6].m_xmf3Position = XMFLOAT3(770.0f, 510.0f, 410.0f); //플레이어 위
	m_pLights[6].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_pLights[6].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[6].m_fFalloff = 8.0f;
	m_pLights[6].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[6].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[7].m_bEnable = true;
	m_pLights[7].m_nType = SPOT_LIGHT;
	m_pLights[7].m_fRange = 300.0f;
	m_pLights[7].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[7].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[7].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[7].m_xmf3Position = XMFLOAT3(620.0f, 510.0f, 675.0f); //플레이어 위
	m_pLights[7].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_pLights[7].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[7].m_fFalloff = 8.0f;
	m_pLights[7].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[7].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[8].m_bEnable = true;
	m_pLights[8].m_nType = POINT_LIGHT;
	m_pLights[8].m_fRange = 300.0f;
	m_pLights[8].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[8].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[8].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[8].m_xmf3Position = XMFLOAT3(190.0f, 710.0f, 235.0f); //플레이어 위
	m_pLights[8].m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pLights[8].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[8].m_fFalloff = 8.0f;
	


	//m_pLights[0].m_bEnable = true;
	//m_pLights[0].m_nType = POINT_LIGHT;
	//m_pLights[0].m_fRange = 1000.0f;
	//m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	//m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	//m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	//m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	//m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);

	//m_pLights[1].m_bEnable = true;
	//m_pLights[1].m_nType = SPOT_LIGHT;
	//m_pLights[1].m_fRange = 500.0f;
	//m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	//m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	//m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	//m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pLights[1].m_fFalloff = 8.0f;
	//m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	//m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	//m_pLights[2].m_bEnable = true;
	//m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	//m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	//m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);

	//m_pLights[3].m_bEnable = true;
	//m_pLights[3].m_nType = SPOT_LIGHT;
	//m_pLights[3].m_fRange = 600.0f;
	//m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	//m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	//m_pLights[3].m_xmf3Position = XMFLOAT3(50.0f, 30.0f, 30.0f);
	//m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	//m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pLights[3].m_fFalloff = 8.0f;
	//m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	//m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));

	//m_pLights[4].m_bEnable = true;
	//m_pLights[4].m_nType = DIRECTIONAL_LIGHT;
	//m_pLights[4].m_fRange = 1500.0f;
	////m_pLights[4].m_xmcolo = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	//m_pLights[4].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pLights[4].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	//m_pLights[4].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	//m_pLights[4].m_xmf3Position = XMFLOAT3(610.0f, 800.0f, 675.0f);
	//m_pLights[4].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, 0.0f);
	//m_pLights[4].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pLights[4].m_fFalloff = 8.0f;
	//m_pLights[4].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	//m_pLights[4].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
//
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 60); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()
	//CreateShaderResourceViews();
	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature); 
//
	BuildDefaultLightsAndMaterials();

	XMFLOAT3 xmf3Scale(1.0f, 1.0f, 1.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.3f, 0.0f, 0.0f);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Terrain/Map1.raw"), 257, 257, xmf3Scale, xmf4Color);
	//m_pPlayGroundTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Terrain/Map1.raw"), 257, 257, xmf3Scale, xmf4Color);

	m_nShaders = 3;
	m_ppShaders = new CShader*[m_nShaders];

	/*CHellicopterObjectsShader *pHellicopterObjectsShader = new CHellicopterObjectsShader();
	pHellicopterObjectsShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pHellicopterObjectsShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pHellicopterObjectsShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pTerrain);*/

	//m_ppShaders[0] = pHellicopterObjectsShader;
	CMapObjectsShader *pPlayGroundObjectsShader = new CMapObjectsShader();
	pPlayGroundObjectsShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pPlayGroundObjectsShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pPlayGroundObjectsShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pTerrain);

	m_ppShaders[0] = pPlayGroundObjectsShader;

	CLoadedModelInfo *pPlayGroundMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALLPIECES.bin", NULL, false);

	CMapObjectsShader *pAngrybotObjectsShader = new CMapObjectsShader();
	pAngrybotObjectsShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pAngrybotObjectsShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pAngrybotObjectsShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pTerrain);

	m_ppShaders[1] = pAngrybotObjectsShader;
 	CLoadedModelInfo *pMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALL.bin", NULL, false);
	
	//CLoadedModelInfo *pPlayGroundMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALLPIECES.bin", NULL, false);

	CUiShader *pUIShader = new CUiShader();
	pUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//pUIShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[2] = pUIShader;
	//CLoadedModelInfo *pMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALL.bin", NULL, false);

	

	/*CMapObjectsShader *pConcertMapShader = new CMapObjectsShader();
	pConcertMapShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pConcertMapShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pConcertMapShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pTerrain);

	m_ppShaders[1] = pConcertMapShader;
	CLoadedModelInfo *pConcertMap = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Player.bin", NULL, true);
*/
	
	//CAngrybotObjectsShader *pAngrybotObjectsShader = new CAngrybotObjectsShader();
	//	pAngrybotObjectsShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//	pAngrybotObjectsShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	/*CMapObjectsShader *pMapObjectsShader = new CMapObjectsShader();
	pMapObjectsShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pMapObjectsShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pMapObjectsShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pTerrain);
	m_ppShaders[2] = pMapObjectsShader;
	
	CLoadedModelInfo *pMapObjectModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Map/ALL_PIECES.bin",NULL,false);
	
	*/
	/*if (!PLAYER->GetPlayer()->m_p)
		PLAYER->GetPlayer()->SetOOBB(PLAYER->GetPlayer()->m_pMesh->GetAABBCenter(), PLAYER->GetPlayer()->m_pMesh->GetAABBExtents(), XMFLOAT4(0, 0, 0, 1));
*/
	
	
	//try {
	//	PLAYER->GetPlayer()->SetOOBB(PLAYER->GetPlayer()->GetPosition(), PLAYER->GetPlayer()->m_pMesh->GetAABBExtents(), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	//}
	//catch (exception e) {
	//	auto error = e.what();
	//	error = error;
	//}

	m_nGameObjects = 2;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];
	
	
	/*m_ppGameObjects[0] = new CAngrybotObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppGameObjects[0]->SetChild(pAngrybotModel->m_pModelRootObject, true);
	m_ppGameObjects[0]->m_pAnimationController = new CAnimationController(2, pAngrybotModel->m_pAnimationSets);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackAnimationSet(0, 0);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackAnimationSet(1, 1);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackWeight(0, 0.8f);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackWeight(1, 0.2f);
	m_ppGameObjects[0]->m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pAngrybotModel);
	m_ppGameObjects[0]->SetPosition(400.0f, m_pTerrain->GetHeight(400.0f, 700.0f), 700.0f);
	m_ppGameObjects[0]->SetScale(2.0f, 2.0f, 2.0f);*/

	m_ppGameObjects[0] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppGameObjects[0]->SetChild(pPlayGroundMapObject->m_pModelRootObject, true);
	//m_ppGameObjects[0]->AddRef();
	//m_ppGameObjects[0]->SetMesh(pAngrybotModel->m_nSkinnedMeshes);
	/*m_ppGameObjects[0]->m_pAnimationController = new CAnimationController(2, pAngrybotModel->m_pAnimationSets);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackAnimationSet(0, 0);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackAnimationSet(1, 1);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackWeight(0, 0.8f);
	m_ppGameObjects[0]->m_pAnimationController->SetTrackWeight(1, 0.2f);
	m_ppGameObjects[0]->m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pAngrybotModel);
	*/
	m_ppGameObjects[0]->SetPosition(200.0f, 255, 700.0f); //맵 거꾸로 버그 여기
	m_ppGameObjects[0]->SetScale(100.0f, 100.0f, 100.0f);
	//if(m_ppGameObjects[0]->m_pMesh)
	m_ppGameObjects[0]->SetMesh(pPlayGroundMapObject->m_pModelRootObject->m_pMesh);

	m_ppGameObjects[0]->SetOOBB(m_ppGameObjects[0]->GetPosition(), Vector3::ScalarProduct( m_ppGameObjects[0]->m_pMesh->GetAABBExtents(),100), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//2번쨰 맵
	m_ppGameObjects[1] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppGameObjects[1]->SetChild(pMapObject->m_pModelRootObject, true);
	m_ppGameObjects[1]->SetPosition(200.0f, 235, 700.0f); //맵 거꾸로 버그 여기
	m_ppGameObjects[1]->SetScale(50.0f, 50.0f, 50.0f);
	//if (m_ppGameObjects[1]->m_pMesh)
	m_ppGameObjects[1]->SetMesh(pMapObject->m_pModelRootObject->m_pMesh);

	m_ppGameObjects[1]->SetOOBB(m_ppGameObjects[1]->GetPosition(), Vector3::ScalarProduct( m_ppGameObjects[1]->m_pMesh->GetAABBExtents(),50), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//CLoadedModelInfo *pOtherPlayerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/basstest.bin", NULL, true);
	//m_ppGameObjects[2] = new CAngrybotObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//m_ppGameObjects[2]->SetChild(pOtherPlayerModel->m_pModelRootObject, true);
	//m_ppGameObjects[2]->SetPosition(XMFLOAT3(0, 0, 0)); //맵 거꾸로 버그 여기
	//m_ppGameObjects[2]->SetScale(50.0f, 50.0f, 50.0f);
	//if (m_ppGameObjects[2]->m_pMesh)
	//	m_ppGameObjects[2]->SetOOBB(m_ppGameObjects[2]->GetPosition(), m_ppGameObjects[2]->m_pMesh->GetAABBExtents(), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	//m_ppGameObjects[2]->m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pOtherPlayerModel);
	//m_ppGameObjects[2]->m_pAnimationController = new CAnimationController(1, pOtherPlayerModel->m_pAnimationSets);
	//m_ppGameObjects[2]->m_pAnimationController->SetTrackAnimationSet(0, 0);
	//m_ppGameObjects[2]->m_pAnimationController->SetCallbackKeys(1, 3);

	//m_ppGameObjects[1] = new CAngrybotObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//m_ppGameObjects[1]->SetChild(pAngrybotModel->m_pModelRootObject, true);
	//m_ppGameObjects[1]->m_pAnimationController = new CAnimationController(1, pAngrybotModel->m_pAnimationSets);
	//m_ppGameObjects[1]->m_pAnimationController->SetTrackAnimationSet(0, 1);
	//m_ppGameObjects[1]->m_pAnimationController->SetTrackSpeed(0, 0.25f);
	//m_ppGameObjects[1]->m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pAngrybotModel);
	//m_ppGameObjects[1]->SetPosition(450.0f, m_pTerrain->GetHeight(450.0f, 680.0f), 680.0f);
	//m_ppGameObjects[1]->SetScale(2.0f, 2.0f, 2.0f);


	//PLAYER->AddPlayer()
	m_nPlayGroundObjects = 14;
	m_ppPlayGroundObjects = new CGameObject*[m_nPlayGroundObjects];

	CLoadedModelInfo *Floor = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Floor.bin", NULL, false);
	m_ppPlayGroundObjects[0] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[0]->SetChild(Floor->m_pModelRootObject, true);
	m_ppPlayGroundObjects[0]->SetPosition(0.0f, 0, 0.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[0]->SetScale(10.0f, 10.0f, 10.0f);
	m_ppPlayGroundObjects[0]->SetMesh(Floor->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[0]->SetOOBB(m_ppPlayGroundObjects[0]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[0]->m_pMesh->GetAABBExtents(),60.46), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Floor_Slide = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Floor_Slide.bin", NULL, false);
	m_ppPlayGroundObjects[1] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[1]->SetChild(Floor_Slide->m_pModelRootObject, true);
	m_ppPlayGroundObjects[1]->SetPosition(0.0f, 10, 0.0f);
	m_ppPlayGroundObjects[1]->SetScale(10.0f, 10.0f, 10.0f);
	m_ppPlayGroundObjects[1]->SetMesh(Floor_Slide->m_pModelRootObject->m_pMesh);
	//if (m_ppPlayGroundObjects[1]->m_pMesh)
	m_ppPlayGroundObjects[1]->SetOOBB(m_ppPlayGroundObjects[1]->GetPosition(), XMFLOAT3(350, 20, 350), XMFLOAT4(0, 0, 0, 1));//Vector3::ScalarProduct(m_ppPlayGroundObjects[1]->m_pMesh->GetAABBExtents(),10*objScale) , XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Floor_Stage = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/바닥.bin", NULL, false);
	m_ppPlayGroundObjects[2] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[2]->SetChild(Floor_Stage->m_pModelRootObject, true);
	m_ppPlayGroundObjects[2]->SetPosition(0.0f, 20, -825.0f); 
	m_ppPlayGroundObjects[2]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[2]->SetMesh(Floor_Stage->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[2]->SetOOBB(m_ppPlayGroundObjects[2]->GetPosition(),XMFLOAT3( m_ppPlayGroundObjects[2]->m_pMesh->GetAABBExtents().x * objScale * 20, m_ppPlayGroundObjects[2]->m_pMesh->GetAABBExtents().y * objScale * 10, m_ppPlayGroundObjects[2]->m_pMesh->GetAABBExtents().z * objScale * 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//
	CLoadedModelInfo *Slide = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/미끄럼틀.bin", NULL, false);
	m_ppPlayGroundObjects[3] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[3]->SetChild(Slide->m_pModelRootObject, true);
	m_ppPlayGroundObjects[3]->SetPosition(0.0f, 20, 0.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[3]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[3]->SetMesh(Slide->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[3]->SetOOBB(m_ppPlayGroundObjects[3]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[3]->m_pMesh->GetAABBExtents(),20 * objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Respawn_Blue = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Respawn_Blue.bin", NULL, false);
	m_ppPlayGroundObjects[4] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[4]->SetChild(Respawn_Blue->m_pModelRootObject, true);
	m_ppPlayGroundObjects[4]->SetPosition(1020.0f, 30, 700.0f); //ㅇ
	m_ppPlayGroundObjects[4]->SetScale(10.0f, 10.0f, 10.0f);
	m_ppPlayGroundObjects[4]->SetMesh(Respawn_Blue->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[4]->SetOOBB(m_ppPlayGroundObjects[4]->GetPosition(), XMFLOAT3(m_ppPlayGroundObjects[4]->m_pMesh->GetAABBExtents().x * objScale * 10, m_ppPlayGroundObjects[4]->m_pMesh->GetAABBExtents().y * objScale * 10, m_ppPlayGroundObjects[4]->m_pMesh->GetAABBExtents().z * objScale * 75), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Respawn_Red = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Respawn_Red.bin", NULL, false);
	m_ppPlayGroundObjects[5] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[5]->SetChild(Respawn_Red->m_pModelRootObject, true);
	m_ppPlayGroundObjects[5]->SetPosition(-1030.0f, 30, 680.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[5]->SetScale(10.0f, 10.0f, 10.0f);
	m_ppPlayGroundObjects[5]->SetMesh(Respawn_Red->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[5]->SetOOBB(m_ppPlayGroundObjects[5]->GetPosition(), XMFLOAT3(m_ppPlayGroundObjects[5]->m_pMesh->GetAABBExtents().x * objScale * 10, m_ppPlayGroundObjects[5]->m_pMesh->GetAABBExtents().y * objScale * 10, m_ppPlayGroundObjects[5]->m_pMesh->GetAABBExtents().z * objScale * 75), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *LeftSeeSaw = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/SeeSaw.bin", NULL, false);
	m_ppPlayGroundObjects[6] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[6]->SetChild(LeftSeeSaw->m_pModelRootObject, true);
	m_ppPlayGroundObjects[6]->SetPosition(1030.0f, 10, -280.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[6]->SetScale(6.0f, 6.0f, 6.0f);
	m_ppPlayGroundObjects[6]->SetMesh(LeftSeeSaw->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[6]->SetOOBB(m_ppPlayGroundObjects[6]->GetPosition(), XMFLOAT3(m_ppPlayGroundObjects[6]->m_pMesh->GetAABBExtents().x * objScale * 4, m_ppPlayGroundObjects[6]->m_pMesh->GetAABBExtents().y * objScale * 6, m_ppPlayGroundObjects[6]->m_pMesh->GetAABBExtents().z * objScale * 55), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *RightSeeSaw = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/SeeSaw.bin", NULL, false);
	m_ppPlayGroundObjects[7] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[7]->SetChild(RightSeeSaw->m_pModelRootObject, true);
	m_ppPlayGroundObjects[7]->SetPosition(-1030.0f, 10, -280.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[7]->SetScale(6.0f, 6.0f, 6.0f);
	m_ppPlayGroundObjects[7]->SetMesh(RightSeeSaw->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[7]->SetOOBB(m_ppPlayGroundObjects[7]->GetPosition(), XMFLOAT3(m_ppPlayGroundObjects[7]->m_pMesh->GetAABBExtents().x * objScale * 4, m_ppPlayGroundObjects[7]->m_pMesh->GetAABBExtents().y * objScale * 6, m_ppPlayGroundObjects[7]->m_pMesh->GetAABBExtents().z * objScale * 55), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//충돌여기

	CLoadedModelInfo *Slide_Speaker_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Slide_Speaker_3.bin", NULL, false);
	m_ppPlayGroundObjects[8] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[8]->SetChild(Slide_Speaker_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[8]->SetPosition(40.0f, 45, -150.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[8]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[8]->SetMesh(Slide_Speaker_3->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[8]->SetOOBB(m_ppPlayGroundObjects[8]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[8]->m_pMesh->GetAABBExtents(),20 * objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Slide_Speaker_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Slide_Speaker_1.bin", NULL, false);
	m_ppPlayGroundObjects[9] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[9]->SetChild(Slide_Speaker_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[9]->SetPosition(10.0f, 45, 100.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[9]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[9]->SetMesh(Slide_Speaker_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[9]->SetOOBB(m_ppPlayGroundObjects[9]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[9]->m_pMesh->GetAABBExtents(), 20* objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Stage = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Stage.bin", NULL, false);
	m_ppPlayGroundObjects[10] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[10]->SetChild(Slide_Speaker_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[10]->SetPosition(0.0f, 45, -820.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[10]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[10]->SetMesh(Stage->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[10]->SetOOBB(m_ppPlayGroundObjects[10]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[10]->m_pMesh->GetAABBExtents(), 20 * objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Spring_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Slide_Speaker_1.bin", NULL, false);
	m_ppPlayGroundObjects[11] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[11]->SetChild(Spring_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[11]->SetPosition(500.0f, 45, 100.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[11]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[11]->SetMesh(Spring_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[11]->SetOOBB(m_ppPlayGroundObjects[11]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[11]->m_pMesh->GetAABBExtents(), 20 * objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Spring_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Spring_1.bin", NULL, false);
	m_ppPlayGroundObjects[12] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[12]->SetChild(Spring_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[12]->SetPosition(-700.0f, 20, 600.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[12]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[12]->SetMesh(Spring_2->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[12]->SetOOBB(m_ppPlayGroundObjects[12]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[12]->m_pMesh->GetAABBExtents(), 20 * objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Spring_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Spring_2.bin", NULL, false);
	m_ppPlayGroundObjects[13] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[13]->SetChild(Spring_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[13]->SetPosition(-900.0f, 20, 700.0f); //맵 거꾸로 버그 여기
	m_ppPlayGroundObjects[13]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[13]->SetMesh(Spring_3->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[13]->SetOOBB(m_ppPlayGroundObjects[13]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[13]->m_pMesh->GetAABBExtents(), 20 * objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//UI
	//CTexture *ppUITextures;
	//ppUITextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppUITextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/pg_logo.dds", 0);

	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	//CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // 텍스쳐 srv
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//CreateShaderResourceViews(pd3dDevice, ppUITextures, 3, true);

	//m_nUIObjects = 2;
	//m_ppUIObjects = new CGameObject*[m_nUIObjects];

	//CMaterial *ppUIMaterials;
	//ppUIMaterials = new CMaterial(1);
	//ppUIMaterials->SetTexture(ppUITextures);
	//CTexturedRectMesh *pUIMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 15.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//

	//CBillboardObject *pBillboardObject = NULL;

	//pBillboardObject = new CBillboardObject();

	//pBillboardObject->SetMaterial(1, ppUIMaterials);
	//pBillboardObject->SetPosition(PLAYER->GetPlayer()->GetPosition().x, PLAYER->GetPlayer()->GetPosition().y+30, PLAYER->GetPlayer()->GetPosition().z); // 셋포지션의 헤이트값 조절해서 바닥에서 자라는지 설정. 풀 월드포지션 설정해주는 부분임
	//pBillboardObject->(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));

	 //billboardobj = new CBillboardObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	 //billboardobj->SetPosition(PLAYER->GetPlayer()->GetPosition().x, PLAYER->GetPlayer()->GetPosition().y + 30, PLAYER->GetPlayer()->GetPosition().z);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	if (pMapObject) delete pMapObject;
	if (pPlayGroundMapObject) delete pPlayGroundMapObject;

}

void CScene::BuildObjectsAfterPlayer(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	billboardobj = new CBillboardObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	billboardobj->SetPosition(XMFLOAT3(30,0,-400));

	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();

	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; i++)
		{
			m_ppShaders[i]->ReleaseShaderVariables();
			m_ppShaders[i]->ReleaseObjects();
			m_ppShaders[i]->Release();
		}
		delete[] m_ppShaders;
	}

	if (m_pTerrain) delete m_pTerrain;
	if (m_pSkyBox) delete m_pSkyBox;

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	//ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;
	//D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[10];
	//pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[0].NumDescriptors = 1;
	//pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	//pd3dDescriptorRanges[0].RegisterSpace = 0;
	//pd3escriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[1].NumDescriptors = 1;
	//pd3dDescriptorRanges[1].BaseShaderRegister = 7; //t7: gtxtSpecularTexture
	//pd3dDescriptorRanges[1].RegisterSpace = 0;
	//pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[2].NumDescriptors = 1;
	//pd3dDescriptorRanges[2].BaseShaderRegister = 8; //t8: gtxtNormalTexture
	//pd3dDescriptorRanges[2].RegisterSpace = 0;
	//pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//
	//pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[3].NumDescriptors = 1;
	//pd3dDescriptorRanges[3].BaseShaderRegister = 9; //t9: gtxtMetallicTexture
	//pd3dDescriptorRanges[3].RegisterSpace = 0;
	//pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[4].NumDescriptors = 1;
	//pd3dDescriptorRanges[4].BaseShaderRegister = 10; //t10: gtxtEmissionTexture
	//pd3dDescriptorRanges[4].RegisterSpace = 0;
	//pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//p3dDescriptorRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[5].NumDescriptors = 1;
	//pd3dDescriptorRanges[5].BaseShaderRegister = 11; //t11: gtxtEmissionTexture
	//pd3dDescriptorRanges[5].RegisterSpace = 0;
	//pd3dDescriptorRanges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[6].NumDescriptors = 1;
	//pd3dDescriptorRanges[6].BaseShaderRegister = 12; //t12: gtxtEmissionTexture
	//pd3dDescriptorRanges[6].RegisterSpace = 0;
	//pd3dDescriptorRanges[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[7].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[7].NumDescriptors = 1;
	//pd3dDescriptorRanges[7].BaseShaderRegister = 13; //t13: gtxtSkyBoxTexture
	//pd3dDescriptorRanges[7].RegisterSpace = 0;
	//pd3dDescriptorRanges[7].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[8].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[8].NumDescriptors = 1;
	//pd3dDescriptorRanges[8].BaseShaderRegister = 1; //t1: gtxtTerrainBaseTexture
	//pd3dDescriptorRanges[8].RegisterSpace = 0;
	//pd3dDescriptorRanges[8].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//pd3dDescriptorRanges[9].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[9].NumDescriptors = 1;
	//pd3dDescriptorRanges[9].BaseShaderRegister = 2; //t2: gtxtTerrainDetailTexture
	//pd3dDescriptorRanges[9].RegisterSpace = 0;
	//pd3dDescriptorRanges[9].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	////pd3dDescriptorRanges[10].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	////pd3dDescriptorRanges[10].NumDescriptors = 1;
	////pd3dDescriptorRanges[10].BaseShaderRegister = 14; //t2: gtxtTerrainDetailTexture 게임오브젲ㄱ트니까
	////pd3dDescriptorRanges[10].RegisterSpace = 0;
	////pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	////pd3dDescriptorRanges[10].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	////pd3dDescriptorRanges[10].NumDescriptors = 1;						//m_nScreenOffRederTarget의 개수
	////pd3dDescriptorRanges[10].BaseShaderRegister = 20; //T20: gtxtScene
	////pd3dDescriptorRanges[10].RegisterSpace = 0;
	////pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//D3D12_ROOT_PARAMETER pd3dRootParameters[15];
	//pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	//pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	//pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	//pd3dRootParameters[1].Constants.Num32BitValues = 33;
	//pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	//pd3dRootParameters[1].Constants.RegisterSpace = 0;
	//pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	//pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	//pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[3].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[0]);
	//pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[4].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[1]);
	//pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[2]);
	//pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[3]);
	//pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[7].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[4]);
	//pd3dRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[8].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[5]);
	//pd3dRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[9].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[9].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[6]);
	//pd3dRootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[10].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[10].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[7]);
	//pd3dRootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pd3dRootParameters[11].Descriptor.ShaderRegister = 7; //Skinned Bone Offsets
	//pd3dRootParameters[11].Descriptor.RegisterSpace = 0;
	//pd3dRootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//pd3dRootParameters[12].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pd3dRootParameters[12].Descriptor.ShaderRegister = 8; //Skinned Bone Transforms
	//pd3dRootParameters[12].Descriptor.RegisterSpace = 0;
	//pd3dRootParameters[12].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//pd3dRootParameters[13].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[13].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[13].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[8]);
	//pd3dRootParameters[13].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[14].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[14].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[14].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[9]);
	//pd3dRootParameters[14].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	////pd3dRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////pd3dRootParameters[15].Descriptor.ShaderRegister = 9;
	////pd3dRootParameters[15].Descriptor.RegisterSpace = 0;
	////pd3dRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	////pd3dRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	////pd3dRootParameters[15].DescriptorTable.NumDescriptorRanges = 1;
	////pd3dRootParameters[15].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[10];				//t21 : UI
	////pd3dRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//D3D12_STATIC_SAMPLER_DESC pd3dSamplerDescs[2];
	//pd3dSamplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//pd3dSamplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//pd3dSamplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//pd3dSamplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//pd3dSamplerDescs[0].MipLODBias = 0;
	//pd3dSamplerDescs[0].MaxAnisotropy = 1;
	//pd3dSamplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//pd3dSamplerDescs[0].MinLOD = 0;
	//pd3dSamplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	//pd3dSamplerDescs[0].ShaderRegister = 0;
	//pd3dSamplerDescs[0].RegisterSpace = 0;
	//pd3dSamplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//pd3dSamplerDescs[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//pd3dSamplerDescs[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//pd3dSamplerDescs[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//pd3dSamplerDescs[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//pd3dSamplerDescs[1].MipLODBias = 0;
	//pd3dSamplerDescs[1].MaxAnisotropy = 1;
	//pd3dSamplerDescs[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//pd3dSamplerDescs[1].MinLOD = 0;
	//pd3dSamplerDescs[1].MaxLOD = D3D12_FLOAT32_MAX;
	//pd3dSamplerDescs[1].ShaderRegister = 1;
	//pd3dSamplerDescs[1].RegisterSpace = 0;
	//pd3dSamplerDescs[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	//D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	//::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	//d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	//d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	//d3dRootSignatureDesc.NumStaticSamplers = _countof(pd3dSamplerDescs);
	//d3dRootSignatureDesc.pStaticSamplers = pd3dSamplerDescs;
	//d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	//ID3DBlob *pd3dSignatureBlob = NULL;
	//ID3DBlob *pd3dErrorBlob = NULL;
	//D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	//pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	//if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	//if (pd3dErrorBlob) pd3dErrorBlob->Release();

	//return(pd3dGraphicsRootSignature);
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[12];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 7; //t7: gtxtSpecularTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 8; //t8: gtxtNormalTexture
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[3].NumDescriptors = 1;
	pd3dDescriptorRanges[3].BaseShaderRegister = 9; //t9: gtxtMetallicTexture
	pd3dDescriptorRanges[3].RegisterSpace = 0;
	pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[4].NumDescriptors = 1;
	pd3dDescriptorRanges[4].BaseShaderRegister = 10; //t10: gtxtEmissionTexture
	pd3dDescriptorRanges[4].RegisterSpace = 0;
	pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[5].NumDescriptors = 1;
	pd3dDescriptorRanges[5].BaseShaderRegister = 11; //t11: gtxtEmissionTexture
	pd3dDescriptorRanges[5].RegisterSpace = 0;
	pd3dDescriptorRanges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[6].NumDescriptors = 1;
	pd3dDescriptorRanges[6].BaseShaderRegister = 12; //t12: gtxtEmissionTexture
	pd3dDescriptorRanges[6].RegisterSpace = 0;
	pd3dDescriptorRanges[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[7].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[7].NumDescriptors = 1;
	pd3dDescriptorRanges[7].BaseShaderRegister = 13; //t13: gtxtSkyBoxTexture
	pd3dDescriptorRanges[7].RegisterSpace = 0;
	pd3dDescriptorRanges[7].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[8].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[8].NumDescriptors = 1;
	pd3dDescriptorRanges[8].BaseShaderRegister = 1; //t1: gtxtTerrainBaseTexture
	pd3dDescriptorRanges[8].RegisterSpace = 0;
	pd3dDescriptorRanges[8].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[9].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[9].NumDescriptors = 1;
	pd3dDescriptorRanges[9].BaseShaderRegister = 2; //t2: gtxtTerrainDetailTexture
	pd3dDescriptorRanges[9].RegisterSpace = 0;
	pd3dDescriptorRanges[9].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[10].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[10].NumDescriptors = 1;
	pd3dDescriptorRanges[10].BaseShaderRegister = 14; //t2: gtxtTerrainDetailTexture
	pd3dDescriptorRanges[10].RegisterSpace = 0;
	pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[11].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[11].NumDescriptors = 1;
	pd3dDescriptorRanges[11].BaseShaderRegister = 15; //t15
	pd3dDescriptorRanges[11].RegisterSpace = 0;
	pd3dDescriptorRanges[11].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//pd3dDescriptorRanges[10].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[10].NumDescriptors = 1;						//m_nScreenOffRederTarget의 개수
	//pd3dDescriptorRanges[10].BaseShaderRegister = 20; //T20: gtxtScene
	//pd3dDescriptorRanges[10].RegisterSpace = 0;
	//pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	D3D12_ROOT_PARAMETER pd3dRootParameters[17];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 33;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[3].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[0]);
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[4].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[1]);
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[2]);
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[3]);
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[7].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[4]);
	pd3dRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[8].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[5]);
	pd3dRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[9].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[9].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[6]);
	pd3dRootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[10].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[10].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[7]);
	pd3dRootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[11].Descriptor.ShaderRegister = 7; //Skinned Bone Offsets
	pd3dRootParameters[11].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[12].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[12].Descriptor.ShaderRegister = 8; //Skinned Bone Transforms
	pd3dRootParameters[12].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[12].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[13].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[13].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[13].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[8]);
	pd3dRootParameters[13].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[14].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[14].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[14].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[9]);
	pd3dRootParameters[14].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[15].Descriptor.ShaderRegister = 5;
	pd3dRootParameters[15].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[16].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[16].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[16].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[11]);
	pd3dRootParameters[16].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//pd3dRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[15].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[15].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[10];				//t21 : UI
	//pd3dRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC pd3dSamplerDescs[2];

	pd3dSamplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].MipLODBias = 0;
	pd3dSamplerDescs[0].MaxAnisotropy = 1;
	pd3dSamplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[0].MinLOD = 0;
	pd3dSamplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[0].ShaderRegister = 0;
	pd3dSamplerDescs[0].RegisterSpace = 0;
	pd3dSamplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dSamplerDescs[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].MipLODBias = 0;
	pd3dSamplerDescs[1].MaxAnisotropy = 1;
	pd3dSamplerDescs[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[1].MinLOD = 0;
	pd3dSamplerDescs[1].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[1].ShaderRegister = 1;
	pd3dSamplerDescs[1].RegisterSpace = 0;
	pd3dSamplerDescs[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(pd3dSamplerDescs);
	d3dRootSignatureDesc.pStaticSamplers = pd3dSamplerDescs;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);

}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	if (m_pSkyBox) m_pSkyBox->ReleaseUploadBuffers();
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();

	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->ReleaseUploadBuffers();
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseUploadBuffers();
}

void CScene::SetCollideBox()
{
	
}

void CScene::CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorNextHandle = m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorNextHandle = m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorNextHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorNextHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

D3D12_GPU_DESCRIPTOR_HANDLE CScene::CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = m_d3dCbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		m_d3dCbvCPUDescriptorNextHandle.ptr = m_d3dCbvCPUDescriptorNextHandle.ptr + ::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_d3dCbvCPUDescriptorNextHandle);
		m_d3dCbvGPUDescriptorNextHandle.ptr = m_d3dCbvGPUDescriptorNextHandle.ptr + ::gnCbvSrvDescriptorIncrementSize;
	}
	return(d3dCbvGPUDescriptorHandle);
}

D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(D3D12_RESOURCE_DESC d3dResourceDesc, UINT nTextureType)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	switch (nTextureType)
	{
		case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
		case RESOURCE_TEXTURE2D_ARRAY:
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
			d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
			d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
			d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
			d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
			d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
			d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
			break;
		case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			d3dShaderResourceViewDesc.TextureCube.MipLevels = -1;
			d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			break;
		case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
			d3dShaderResourceViewDesc.Buffer.NumElements = 0;
			d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
			d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			break;
	}
	return(d3dShaderResourceViewDesc);
}

D3D12_GPU_DESCRIPTOR_HANDLE CScene::CreateShaderResourceViews(ID3D12Device *pd3dDevice, CTexture *pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorNextHandle;
	if (pTexture)
	{
		int nTextures = pTexture->GetTextures();
		int nTextureType = pTexture->GetTextureType();
		for (int i = 0; i < nTextures; i++)
		{
			ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
			D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);
			m_d3dSrvCPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameter + i) : nRootParameter, m_d3dSrvGPUDescriptorNextHandle);
			m_d3dSrvGPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		}
	}
	return(d3dSrvGPUDescriptorHandle);
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
	/*	case 'W': m_ppGameObjects[0]->MoveForward(+3.0f); break;
		case 'S': m_ppGameObjects[0]->MoveForward(-3.0f); break;
		case 'A': m_ppGameObjects[0]->MoveStrafe(-3.0f); break;
		case 'D': m_ppGameObjects[0]->MoveStrafe(+3.0f); break;
		case 'Q': m_ppGameObjects[0]->MoveUp(+3.0f); break;
		case 'R': m_ppGameObjects[0]->MoveUp(-3.0f); break;*/
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	for (int i = 0; i < m_nShaders; i++) if (m_ppShaders[i]) m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	
	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = PLAYER->GetPlayer()->GetPosition();
		m_pLights[1].m_xmf3Direction = PLAYER->GetPlayer()->GetLookVector();
	}

	if (billboardobj) billboardobj->Animate(fTimeElapsed, PLAYER->GetPlayer()->GetCamera());
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	UpdateShaderVariables(pd3dCommandList);
	billboardobj->UpdateShaderVariable(pd3dCommandList);

//	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
//	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights
//
//	if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);
//	
//	//if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);
//
//	for (int i = 0; i < m_nGameObjects; i++)
//	{
//		if (m_ppGameObjects[i])
//		{
////			m_ppGameObjects[i]->Animate(m_fElapsedTime);
//			m_ppGameObjects[i]->UpdateTransform(NULL);
//			//if (m_MapNum == 0)
//			{
//				m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
//			}
//		}
//	}

	/*switch (SCENEMANAGER->GetSceneType())
	{
	case MENUSCENE:
		m_ppGameObjects[0]->Render(pd3dCommandList, pCamera);
		break;
	case INGAME:
		m_ppGameObjects[1]->Render(pd3dCommandList, pCamera);
		break;
	}*/

	for (int i = 0; i < m_nPlayGroundObjects; i++)
	{
		m_ppPlayGroundObjects[i]->Render(pd3dCommandList, pCamera);
	}

	//if (billboardobj)
	//{
	//	//billboardobj->SetLookAt(pCamera->GetPosition());
	//	billboardobj->Render(pd3dCommandList, pCamera);
	//}
	//m_ppGameObjects[2]->Render(pd3dCommandList, pCamera);

	/*if (m_ppGameObjects[SCENEMANAGER->GetSceneType()])
		m_ppGameObjects[SCENEMANAGER->GetSceneType()]->Render(pd3dCommandList, pCamera);*/
	//XMFLOAT3 pos = PLAYER->GetPlayer()->GetPosition();
	//XMFLOAT3 MAPpos = m_ppGameObjects[0]->GetPosition();
	//XMFLOAT3 otherplayerpos = m_ppGameObjects[2]->GetPosition();

	/*for (auto p = PLAYER->m_vecPlayerList.begin(); p!= PLAYER->m_vecPlayerList.end(); ++p)
	{
		(*p)->Render(pd3dCommandList, pCamera);
	}*/
	/*for (int i = 0; i < PLAYER->GetPlayerMap()[ENEMY].size(); ++i)
	{
		PLAYER->GetPlayerMap()[ENEMY][i]->Render(pd3dCommandList, pCamera);
	}*/
	/*for (int i = 0; i < OBJECTMANAGER->GetObjlist[MAP].size(); ++i)
	{
		
		for (auto iter = OBJECTMANAGER->GetObjlist[MAP]->begin(); iter != OBJECTMANAGER->GetObjlist[MAP].end(); ++iter)
		{
			
		}
	}*/
	for (int i = 0; i < m_nShaders; i++) if (m_ppShaders[i]) m_ppShaders[i]->Render(pd3dCommandList, pCamera);
	//CheckObjectByObjectCollisions();

}

void CScene::CheckObjectByObjectCollisions() {
	//체크는되는듯한데 왜 중단점이 안되지?
	
	//for (int i = 1; i < m_nPlayGroundObjects; i++)
	//{
	////	if (m_ppPlayGroundObjects[i])
	////	{
	//	m_ppPlayGroundObjects[i]->Animate(m_fElapsedTime);
	//m_ppPlayGroundObjects[i]->UpdateTransform(NULL);
	//m_ppPlayGroundObjects[i]->GetBoundingBox().Center = m_ppPlayGroundObjects[i]->GetPosition();
	////	}
	//}
	//static BoundingOrientedBox box = PLAYER->GetPlayer()->GetBoundingBox();
	//
	for (int i = 1; i < m_nPlayGroundObjects; i++)
	{
		if (m_ppPlayGroundObjects[i])
		{
			BoundingOrientedBox Mapbox = m_ppPlayGroundObjects[i]->GetBoundingBox();

			if (m_ppPlayGroundObjects[i]->GetBoundingBox().Intersects(PLAYER->GetPlayer()->GetBoundingBox()))
			{
				//PLAYER->GetPlayer()->SetPosition()
				//PLAYER->GetPlayer()->SetAllowKey(false);
				//PLAYER->GetPlayer()->SetVelocity(XMFLOAT3(0,0,0));

				PLAYER->GetPlayer()->SetPlayCrashMap(true);
				//PLAYER->GetPlayer()->SetVelocity( PLAYER->GetPlayer()->GetVelocity().x - 0.5);
				//int i = 0;
			}
			else
			{
				PLAYER->GetPlayer()->SetPlayCrashMap(false);

				//PLAYER->GetPlayer()->SetAllowKey(true);
			}
		}
	}

	
	//PLAYER->GetPlayer()->SetCollimdeBox();

	//XMFLOAT3 playeroobb = PLAYER->GetPlayer()->GetBoundingBox().Center;
	//XMFLOAT3 playeroobbex = PLAYER->GetPlayer()->GetBoundingBox().Extents;

	//XMFLOAT3 objoobb = m_ppGameObjects[SCENEMANAGER->GetSceneType()]->GetBoundingBox().Center;
	//XMFLOAT3 objoobbex = m_ppGameObjects[SCENEMANAGER->GetSceneType()]->GetBoundingBox().Extents;
	//
	//XMFLOAT3 tempCenter = XMFLOAT3(playeroobb.x + objoobbex.x,
	//	playeroobb.y + objoobbex.y,
	//	playeroobb.z + objoobbex.z);



	////PLAYER->GetPlayer()->SetOOBB(tempCenter, playeroobbex, XMFLOAT4(0, 0, 0, 1));
	//m_ppGameObjects[SCENEMANAGER->GetSceneType()]->SetOOBB(objoobb, playeroobbex, XMFLOAT4(0, 0, 0, 1));
	//
	//auto playerBox = PLAYER->GetPlayer()->GetBoundingBox();
	//auto objBox = m_ppGameObjects[SCENEMANAGER->GetSceneType()]->GetBoundingBox();

	//if (playerBox.Contains(objBox))
	//{
	//	PLAYER->GetPlayer()->SetAllowKey(false);
	//}
	//else
	//{
	//	PLAYER->GetPlayer()->SetAllowKey(true);
	//}
	//BoundingOrientedBox Mapbox = m_ppGameObjects[SCENEMANAGER->GetSceneType()]->GetBoundingBox();

	//if (PLAYER->GetPlayer()->m_xmOOBB.Intersects(m_ppGameObjects[SCENEMANAGER->GetSceneType()]->m_xmOOBB)){
	//	
	//	int i = 0;
	//	//PLAYER->GetPlayer()->SetPosition(XMFLOAT3(900.0f, 900.0f, 900.0f));
	//}
	

}

