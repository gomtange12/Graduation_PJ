//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "CPlayerManager.h"
#include "CSceneManager.h"
#include "CObjectManager.h"
#include "CBillboardObject.h"
#include "CNetWork.h"
#define PG_objScale 2.54
#define CONCERTScaale 2.54 * 50
#define objScale 6.47
#define CONCERTScale 250

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

	//Áß¾Ó¶óÀÌÆ®
	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = SPOT_LIGHT;
	m_pLights[0].m_fRange = 300.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(1500.0f, 50.0f, 625.0f); //ÇÃ·¹ÀÌ¾î À§
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
	m_pLights[1].m_xmf3Position = XMFLOAT3(510.0f, 30.0f, 199.0f); //ÇÃ·¹ÀÌ¾î À§
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
	m_pLights[2].m_xmf3Position = XMFLOAT3(2550, 30.0f, 119.0f); //ÇÃ·¹ÀÌ¾î À§
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[2].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[2].m_fFalloff = 8.0f;
	m_pLights[2].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[2].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[3].m_bEnable = false;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 300.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(-180.0f, 510.0f, -260.0f); //ÇÃ·¹ÀÌ¾î À§
	m_pLights[3].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[4].m_bEnable = false;
	m_pLights[4].m_nType = SPOT_LIGHT;
	m_pLights[4].m_fRange = 300.0f;
	m_pLights[4].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[4].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[4].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[4].m_xmf3Position = XMFLOAT3(620.0f, 510.0f, -260.0f); //ÇÃ·¹ÀÌ¾î À§
	m_pLights[4].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_pLights[4].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[4].m_fFalloff = 8.0f;
	m_pLights[4].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[4].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[5].m_bEnable = false;
	m_pLights[5].m_nType = SPOT_LIGHT;
	m_pLights[5].m_fRange = 300.0f;
	m_pLights[5].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[5].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[5].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[5].m_xmf3Position = XMFLOAT3(630.0f, 510.0f, 30.0f); //ÇÃ·¹ÀÌ¾î À§
	m_pLights[5].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_pLights[5].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[5].m_fFalloff = 8.0f;
	m_pLights[5].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[5].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[6].m_bEnable = false;
	m_pLights[6].m_nType = SPOT_LIGHT;
	m_pLights[6].m_fRange = 300.0f;
	m_pLights[6].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[6].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[6].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[6].m_xmf3Position = XMFLOAT3(770.0f, 510.0f, 410.0f); //ÇÃ·¹ÀÌ¾î À§
	m_pLights[6].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_pLights[6].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[6].m_fFalloff = 8.0f;
	m_pLights[6].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[6].m_fTheta = (float)cos(XMConvertToRadians(20.0f));


	m_pLights[7].m_bEnable = false;
	m_pLights[7].m_nType = SPOT_LIGHT;
	m_pLights[7].m_fRange = 300.0f;
	m_pLights[7].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[7].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[7].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[7].m_xmf3Position = XMFLOAT3(620.0f, 510.0f, 675.0f); //ÇÃ·¹ÀÌ¾î À§
	m_pLights[7].m_xmf3Direction = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_pLights[7].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[7].m_fFalloff = 8.0f;
	m_pLights[7].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[7].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[8].m_bEnable = false;
	m_pLights[8].m_nType = POINT_LIGHT;
	m_pLights[8].m_fRange = 300.0f;
	m_pLights[8].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[8].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[8].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[8].m_xmf3Position = XMFLOAT3(190.0f, 710.0f, 235.0f); //ÇÃ·¹ÀÌ¾î À§
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
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 50, 400); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()
	//CreateShaderResourceViews();
	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//
	BuildDefaultLightsAndMaterials();



	XMFLOAT3 xmf3Scale(150.0f, 150.0f, 150.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.3f, 0.0f, 0.0f);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Terrain/Map1.raw"), 257, 257, xmf3Scale, xmf4Color);
	//m_pPlayGroundTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Terrain/Map1.raw"), 257, 257, xmf3Scale, xmf4Color);

	m_nShaders = 15;
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

	CUiShader *pUIShader = new CUiShader();
	pUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[2] = pUIShader;

	//CLoadedModelInfo *pMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALL.bin", NULL, false);
	CUIPlayerShader *pPlayerUIShader = new CUIPlayerShader();
	pPlayerUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pPlayerUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[3] = pPlayerUIShader;

	CUIOtherPlayerShader *pOtherPlayerUIShader = new CUIOtherPlayerShader();
	pOtherPlayerUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pOtherPlayerUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[4] = pOtherPlayerUIShader;

	CUISKillShader *pSkillUIShader = new CUISKillShader();
	pSkillUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pSkillUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[5] = pSkillUIShader;

	CTimeBarShader *pTimeBarUIShader = new CTimeBarShader();
	pTimeBarUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pTimeBarUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[6] = pTimeBarUIShader;

	CWinUIShader *pWinUIShader = new CWinUIShader();
	pWinUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pWinUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[7] = pWinUIShader;

	CLoseUIShader *pLoseUIShader = new CLoseUIShader();
	pLoseUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pLoseUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[8] = pLoseUIShader;

	CHPUIShader *pHpUIShader = new CHPUIShader();
	pHpUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pHpUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	pHpUIShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_ppShaders[9] = pHpUIShader;

	CChatUIShader *pChatUIShader = new CChatUIShader();
	pChatUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pChatUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[10] = pChatUIShader;

	CSkillCoolDownUIShader *psSkillCoolUIShader = new CSkillCoolDownUIShader();
	psSkillCoolUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	psSkillCoolUIShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	psSkillCoolUIShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_ppShaders[11] = psSkillCoolUIShader;

	

	CTimerUIShader *pHunSecShader = new CTimerUIShader();
	pHunSecShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pHunSecShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	pHunSecShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_ppShaders[12] = pHunSecShader;

	CTenSecShader *pTenSecShader = new CTenSecShader();
	pTenSecShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pTenSecShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	pTenSecShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_ppShaders[13] = pTenSecShader;

	COneSecUIShader *pOneSecShader = new COneSecUIShader();
	pOneSecShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pOneSecShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	pOneSecShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_ppShaders[14] = pOneSecShader;

	
	//CSkillEffectUIShader *pEffectShader = new CSkillEffectUIShader();
	//pEffectShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	////pEffectShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	//m_ppShaders[15] = pEffectShader;
	/*CTexturedShader* pOneSecShader = new CTexturedShader();
	pTexturedShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pTexturedShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL);
	m_ppShaders[7] = pTexturedShader;*/


/*Åë¸ÊCMapObjectsShader *pMapObjectsShader = new CMapObjectsShader();
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

/*m_nGameObjects = 2;
m_ppGameObjects = new CGameObject*[m_nGameObjects];
*/

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

//m_ppGameObjects[0] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
//m_ppGameObjects[0]->SetChild(pPlayGroundMapObject->m_pModelRootObject, true);
//
//m_ppGameObjects[0]->SetPosition(200.0f, 255, 700.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
//m_ppGameObjects[0]->SetScale(100.0f, 100.0f, 100.0f);
////if(m_ppGameObjects[0]->m_pMesh)
//m_ppGameObjects[0]->SetMesh(pPlayGroundMapObject->m_pModelRootObject->m_pMesh);

//m_ppGameObjects[0]->SetOOBB(m_ppGameObjects[0]->GetPosition(), Vector3::ScalarProduct( m_ppGameObjects[0]->m_pMesh->GetAABBExtents(),100), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

////2¹ø¤Š ¸Ê
//m_ppGameObjects[1] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
//m_ppGameObjects[1]->SetChild(pMapObject->m_pModelRootObject, true);
//m_ppGameObjects[1]->SetPosition(200.0f, 235, 700.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
//m_ppGameObjects[1]->SetScale(50.0f, 50.0f, 50.0f);
////if (m_ppGameObjects[1]->m_pMesh)
//m_ppGameObjects[1]->SetMesh(pMapObject->m_pModelRootObject->m_pMesh);

//m_ppGameObjects[1]->SetOOBB(m_ppGameObjects[1]->GetPosition(), Vector3::ScalarProduct( m_ppGameObjects[1]->m_pMesh->GetAABBExtents(),50), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));



//PLAYER->AddPlayer()
	m_nPlayGroundObjects = 38;
	m_ppPlayGroundObjects = new CGameObject*[m_nPlayGroundObjects];

   //°íÄ£¹öÁ¯
	CLoadedModelInfo *Floor = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Floor.bin", NULL, false);
	m_ppPlayGroundObjects[0] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[0]->SetChild(Floor->m_pModelRootObject, true);
	m_ppPlayGroundObjects[0]->SetPosition(1500.0f, 0, 1000.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[0]->SetScale(30.0f, 30.0f, 30.0f);
	m_ppPlayGroundObjects[0]->SetMesh(Floor->m_pModelRootObject->m_pMesh);
	//m_ppPlayGroundObjects[0]->SetOOBB(m_ppPlayGroundObjects[0]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[0]->m_pMesh->GetAABBExtents(), 60.46), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[0]->SetOOBB(m_ppPlayGroundObjects[0]->GetPosition(), Vector3::ScalarProduct(m_ppPlayGroundObjects[0]->m_pMesh->GetAABBExtents(), 30 * PG_objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Floor_Slide = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Floor_Slide.bin", NULL, false);
	m_ppPlayGroundObjects[1] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[1]->SetChild(Floor_Slide->m_pModelRootObject, true);
	m_ppPlayGroundObjects[1]->SetPosition(1500.0f, 10, 900.0f);
	m_ppPlayGroundObjects[1]->SetScale(30.0f, 30.0f, 30.0f);
	m_ppPlayGroundObjects[1]->SetMesh(Floor_Slide->m_pModelRootObject->m_pMesh);
	//if (m_ppPlayGroundObjects[1]->m_pMesh)
	//Vector3::ScalarProduct(m_ppPlayGroundObjects[1]->m_pMesh->GetAABBExtents(),10*objScale) , XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
	//m_ppPlayGroundObjects[1]->SetOOBB(XMFLOAT3(1500.0f, 10, 1000.0f), XMFLOAT3(455,10,400), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Floor_Stage = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Floor_Stage.bin", NULL, false);
	m_ppPlayGroundObjects[2] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[2]->SetChild(Floor_Stage->m_pModelRootObject, true);
	m_ppPlayGroundObjects[2]->SetPosition(1500.0f, 20, 780.0f);
	m_ppPlayGroundObjects[2]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[2]->SetMesh(Floor_Stage->m_pModelRootObject->m_pMesh);
	//m_ppPlayGroundObjects[2]->SetOOBB(XMFLOAT3(1500.0f, 10, 105.0f), XMFLOAT3(305,10,130), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Slide = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Slide.bin", NULL, false);
	m_ppPlayGroundObjects[3] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[3]->SetChild(Slide->m_pModelRootObject, true);
	m_ppPlayGroundObjects[3]->SetPosition(1480.0f, 30, 850.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[3]->SetScale(20.0f, 30.0f, 20.0f);
	m_ppPlayGroundObjects[3]->SetMesh(Slide->m_pModelRootObject->m_pMesh);
	//m_ppPlayGroundObjects[3]->SetOOBB(XMFLOAT3(1480.0f, 21, 1000.0f), XMFLOAT3(m_ppPlayGroundObjects[3]->m_pMesh->GetAABBExtents().x * PG_objScale * 18, m_ppPlayGroundObjects[3]->m_pMesh->GetAABBExtents().z * PG_objScale * 19, m_ppPlayGroundObjects[3]->m_pMesh->GetAABBExtents().y * PG_objScale * 10), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Respawn_Blue = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Respawn_Blue.bin", NULL, false);
	m_ppPlayGroundObjects[4] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[4]->SetChild(Respawn_Blue->m_pModelRootObject, true);
	m_ppPlayGroundObjects[4]->SetPosition(1535.0f, 10, 1000.0f); //¤·
	m_ppPlayGroundObjects[4]->SetScale(25.0f, 10.0f, 25.0f);
	m_ppPlayGroundObjects[4]->SetMesh(Respawn_Blue->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *Respawn_Red = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Respawn_Red.bin", NULL, false);
	m_ppPlayGroundObjects[5] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[5]->SetChild(Respawn_Red->m_pModelRootObject, true);
	m_ppPlayGroundObjects[5]->SetPosition(1490.0f, 10, 1000.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[5]->SetScale(25.0f, 10.0f, 25.0f);
	m_ppPlayGroundObjects[5]->SetMesh(Respawn_Red->m_pModelRootObject->m_pMesh);
	

	CLoadedModelInfo *LeftSeeSaw = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/SeeSaw.bin", NULL, false);
	m_ppPlayGroundObjects[6] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[6]->SetChild(LeftSeeSaw->m_pModelRootObject, true);
	m_ppPlayGroundObjects[6]->SetPosition(1900.0f, 10, 1000.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[6]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[6]->SetMesh(LeftSeeSaw->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[6]->SetOOBB(XMFLOAT3(2550.0f, 10, 900.0f), XMFLOAT3(m_ppPlayGroundObjects[6]->m_pMesh->GetAABBExtents().x * objScale * 6, 40, m_ppPlayGroundObjects[6]->m_pMesh->GetAABBExtents().z * objScale * 41), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *RightSeeSaw = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/SeeSaw.bin", NULL, false);
	m_ppPlayGroundObjects[7] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[7]->SetChild(RightSeeSaw->m_pModelRootObject, true);
	m_ppPlayGroundObjects[7]->SetPosition(250.0f, 10, 720.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[7]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[7]->SetMesh(RightSeeSaw->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[7]->SetOOBB(XMFLOAT3(890.0f, 10, 600.0f), XMFLOAT3(m_ppPlayGroundObjects[7]->m_pMesh->GetAABBExtents().x * objScale * 8, 40, m_ppPlayGroundObjects[7]->m_pMesh->GetAABBExtents().z * objScale * 43), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//Ãæµ¹¿©±â

	CLoadedModelInfo *Slide_Speaker_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Slide_Speaker_3.bin", NULL, false);
	m_ppPlayGroundObjects[8] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[8]->SetChild(Slide_Speaker_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[8]->SetPosition(1540.0f, 20, 800.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[8]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[8]->SetMesh(Slide_Speaker_3->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[8]->SetOOBB(XMFLOAT3(1690.0f, 20, 840.0f), XMFLOAT3(m_ppPlayGroundObjects[8]->m_pMesh->GetAABBExtents().x * objScale * 8, 40, m_ppPlayGroundObjects[8]->m_pMesh->GetAABBExtents().z * objScale * 9), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Slide_Speaker_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Slide_Speaker_1.bin", NULL, false);
	m_ppPlayGroundObjects[9] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[9]->SetChild(Slide_Speaker_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[9]->SetPosition(1510.0f, 20, 1100.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[9]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[9]->SetMesh(Slide_Speaker_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[9]->SetOOBB(XMFLOAT3(1510.0f, 20, 1320.0f), XMFLOAT3(m_ppPlayGroundObjects[9]->m_pMesh->GetAABBExtents().x * objScale * 16, 40, m_ppPlayGroundObjects[9]->m_pMesh->GetAABBExtents().z * objScale * 16), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Stage = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Stage.bin", NULL, false);
	m_ppPlayGroundObjects[10] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[10]->SetChild(Stage->m_pModelRootObject, true);
	m_ppPlayGroundObjects[10]->SetPosition(1500.0f, 15.f, 780.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[10]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[10]->SetMesh(Stage->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[10]->SetOOBB(XMFLOAT3(1500.0f, 15.f, 60.0f), XMFLOAT3(m_ppPlayGroundObjects[10]->m_pMesh->GetAABBExtents().x * objScale * 9, m_ppPlayGroundObjects[10]->m_pMesh->GetAABBExtents().y * objScale * 100, m_ppPlayGroundObjects[10]->m_pMesh->GetAABBExtents().z * objScale * 2), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *LeftSpring_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Spring_1.bin", NULL, false);
	m_ppPlayGroundObjects[11] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[11]->SetChild(LeftSpring_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[11]->SetPosition(2360.0f, 20, 2200.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[11]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[11]->SetMesh(LeftSpring_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[11]->SetOOBB(XMFLOAT3(2090.0f, 20, 1620.0f), XMFLOAT3(m_ppPlayGroundObjects[11]->m_pMesh->GetAABBExtents().x * objScale * 10, 20, m_ppPlayGroundObjects[11]->m_pMesh->GetAABBExtents().z * objScale * 10), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Left_Spring_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Spring_2.bin", NULL, false);
	m_ppPlayGroundObjects[12] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[12]->SetChild(Left_Spring_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[12]->SetPosition(2460.0f, 20, 2000.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[12]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[12]->SetMesh(Left_Spring_2->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[12]->SetOOBB(XMFLOAT3(1900.0f, 20, 1775.0f), XMFLOAT3(m_ppPlayGroundObjects[12]->m_pMesh->GetAABBExtents().x * objScale * 14, 20, m_ppPlayGroundObjects[12]->m_pMesh->GetAABBExtents().z * objScale * 9), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Right_Spring_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Spring_1.bin", NULL, false);
	m_ppPlayGroundObjects[13] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[13]->SetChild(Right_Spring_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[13]->SetPosition(1400.0f, 20, 2000.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[13]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[13]->SetMesh(Right_Spring_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[13]->SetOOBB(XMFLOAT3(930.0f, 20, 1560.0f), XMFLOAT3(m_ppPlayGroundObjects[13]->m_pMesh->GetAABBExtents().x * objScale * 8, 20, m_ppPlayGroundObjects[13]->m_pMesh->GetAABBExtents().z * objScale * 8), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *RightSpring_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Spring_2.bin", NULL, false);
	m_ppPlayGroundObjects[14] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[14]->SetChild(RightSpring_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[14]->SetPosition(1500.0f, 20, 2100.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	m_ppPlayGroundObjects[14]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[14]->SetMesh(RightSpring_2->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[14]->SetOOBB(XMFLOAT3(1130.0f, 20, 1730.0f), XMFLOAT3(m_ppPlayGroundObjects[14]->m_pMesh->GetAABBExtents().x * objScale * 10, 20, m_ppPlayGroundObjects[14]->m_pMesh->GetAABBExtents().z * objScale * 9), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Chair_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Chair_1.bin", NULL, false);
	m_ppPlayGroundObjects[15] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[15]->SetChild(Chair_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[15]->Rotate(0.0f, -13.0f, 0.0f);
	m_ppPlayGroundObjects[15]->SetPosition(1100.0f, 10, 800.0f);
	m_ppPlayGroundObjects[15]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[15]->SetMesh(Chair_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[15]->SetOOBB(XMFLOAT3(300.0f, 10, 150.0f), XMFLOAT3(m_ppPlayGroundObjects[15]->m_pMesh->GetAABBExtents().x * objScale * 12, 20, m_ppPlayGroundObjects[15]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Chair_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Chair_1.bin", NULL, false);
	m_ppPlayGroundObjects[16] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[16]->SetChild(Chair_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[16]->Rotate(0.0f, -123.0f, 0.0f);
	m_ppPlayGroundObjects[16]->SetPosition(1770.0f, 10, 600.0f);
	m_ppPlayGroundObjects[16]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[16]->SetMesh(Chair_2->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[16]->SetOOBB(XMFLOAT3(2740.0f, 10, 125.0f), XMFLOAT3(m_ppPlayGroundObjects[16]->m_pMesh->GetAABBExtents().x * objScale * 18, 20, m_ppPlayGroundObjects[16]->m_pMesh->GetAABBExtents().z * objScale * 19), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Street_Lamp_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Street_Lamp_1.bin", NULL, false);
	m_ppPlayGroundObjects[17] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[17]->SetChild(Street_Lamp_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[17]->Rotate(0.0f, -83.0f, 0.0f);
	m_ppPlayGroundObjects[17]->SetPosition(2275.0f, 15, 1100.0f);
	m_ppPlayGroundObjects[17]->SetScale(20.0f, 30.0f, 20.0f);
	m_ppPlayGroundObjects[17]->SetMesh(Street_Lamp_1->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *Street_Lamp_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Street_Lamp_1.bin", NULL, false);
	m_ppPlayGroundObjects[18] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[18]->SetChild(Street_Lamp_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[18]->SetPosition(1225.0f, 15, 675.0f);
	m_ppPlayGroundObjects[18]->SetScale(20.0f, 30.0f, 20.0f);
	m_ppPlayGroundObjects[18]->SetMesh(Street_Lamp_2->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *Stage_Speaker_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Stage_Speaker_1.bin", NULL, false);
	m_ppPlayGroundObjects[19] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[19]->SetChild(Stage_Speaker_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[19]->SetPosition(1500.0f, 20, 775.0f);
	m_ppPlayGroundObjects[19]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[19]->SetMesh(Stage_Speaker_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[19]->SetOOBB(XMFLOAT3(1765.0f, 20, 185.0f), XMFLOAT3(m_ppPlayGroundObjects[19]->m_pMesh->GetAABBExtents().x * objScale * 11, 200, m_ppPlayGroundObjects[19]->m_pMesh->GetAABBExtents().z * objScale * 9), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *Stage_Speaker_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Stage_Speaker_2.bin", NULL, false);
	m_ppPlayGroundObjects[20] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[20]->SetChild(Stage_Speaker_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[20]->SetPosition(1500.0f, 20, 775.0f);
	m_ppPlayGroundObjects[20]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[20]->SetMesh(Stage_Speaker_2->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *Stage_Speaker_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Stage_Speaker_3.bin", NULL, false);
	m_ppPlayGroundObjects[21] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[21]->SetChild(Stage_Speaker_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[21]->SetPosition(1500.0f, 20, 775.0f);
	m_ppPlayGroundObjects[21]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[21]->SetMesh(Stage_Speaker_3->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *Stage_Speaker_4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Stage_Speaker_4.bin", NULL, false);
	m_ppPlayGroundObjects[22] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[22]->SetChild(Stage_Speaker_4->m_pModelRootObject, true);
	m_ppPlayGroundObjects[22]->SetPosition(1500.0f, 20, 775.0f);
	m_ppPlayGroundObjects[22]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[22]->SetMesh(Stage_Speaker_4->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[22]->SetOOBB(XMFLOAT3(1265.0f, 20, 185.0f), XMFLOAT3(m_ppPlayGroundObjects[22]->m_pMesh->GetAABBExtents().x * objScale * 17, 140, m_ppPlayGroundObjects[22]->m_pMesh->GetAABBExtents().z * objScale * 12), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	///////////Á¶¸í
	CLoadedModelInfo *RSpot_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Spot_1.bin", NULL, false);
	m_ppPlayGroundObjects[23] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[23]->SetChild(RSpot_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[23]->SetPosition(1595.0f, 0.f, 1130.0f);
	m_ppPlayGroundObjects[23]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[23]->SetMesh(RSpot_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[23]->SetOOBB(XMFLOAT3(2680.0f, 10.f, 1455.0f), XMFLOAT3(m_ppPlayGroundObjects[23]->m_pMesh->GetAABBExtents().x * objScale * 80, 140, m_ppPlayGroundObjects[23]->m_pMesh->GetAABBExtents().z * objScale * 14), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *RSpot_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ2.bin", NULL, false);
	m_ppPlayGroundObjects[24] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[24]->SetChild(RSpot_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[24]->SetPosition(2320.0f, 60.f, 1330.0f);
	m_ppPlayGroundObjects[24]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[24]->SetMesh(RSpot_2->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *RSpot_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ3.bin", NULL, false);
	m_ppPlayGroundObjects[25] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[25]->SetChild(RSpot_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[25]->SetPosition(1965.0f, 70, 1680.0f);
	m_ppPlayGroundObjects[25]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[25]->SetMesh(RSpot_3->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[25]->SetOOBB(XMFLOAT3(2310.0f, 10.f, 1810.0f), XMFLOAT3(m_ppPlayGroundObjects[25]->m_pMesh->GetAABBExtents().x * objScale * 20, 140, m_ppPlayGroundObjects[25]->m_pMesh->GetAABBExtents().z * objScale * 45), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *RSpot_4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ4.bin", NULL, false);
	m_ppPlayGroundObjects[26] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[26]->SetChild(RSpot_4->m_pModelRootObject, true);
	m_ppPlayGroundObjects[26]->SetPosition(1890.0f, 70, 1920.0f);
	m_ppPlayGroundObjects[26]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[26]->SetMesh(RSpot_4->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *LSpot_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ5.bin", NULL, false);
	m_ppPlayGroundObjects[27] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[27]->SetChild(LSpot_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[27]->SetPosition(80.0f, 70, 1435.0f);
	m_ppPlayGroundObjects[27]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[27]->SetMesh(LSpot_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[27]->SetOOBB(XMFLOAT3(350.0f, 70, 1435.0f), XMFLOAT3(m_ppPlayGroundObjects[27]->m_pMesh->GetAABBExtents().x * objScale * 80, 140, m_ppPlayGroundObjects[27]->m_pMesh->GetAABBExtents().z * objScale * 15), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *LSpot_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ6.bin", NULL, false);
	m_ppPlayGroundObjects[28] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[28]->SetChild(LSpot_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[28]->SetPosition(-145.0f, 70, 1374.0f);
	m_ppPlayGroundObjects[28]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[28]->SetMesh(LSpot_2->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *LSpot_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ7.bin", NULL, false);
	m_ppPlayGroundObjects[29] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[29]->SetChild(LSpot_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[29]->SetPosition(340.0f, 70, 1620.0f);
	m_ppPlayGroundObjects[29]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[29]->SetMesh(LSpot_3->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[29]->SetOOBB(XMFLOAT3(720.0f, 70, 1810.0f), XMFLOAT3(m_ppPlayGroundObjects[29]->m_pMesh->GetAABBExtents().x * objScale * 20, 140, m_ppPlayGroundObjects[29]->m_pMesh->GetAABBExtents().z * objScale * 45), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo *LSpot_4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/½ºÆÌ8.bin", NULL, false);
	m_ppPlayGroundObjects[30] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[30]->SetChild(LSpot_4->m_pModelRootObject, true);
	m_ppPlayGroundObjects[30]->SetPosition(273.0f, 70, 1850.0f);
	m_ppPlayGroundObjects[30]->SetScale(20.0f, 20.0f, 20.0f);
	m_ppPlayGroundObjects[30]->SetMesh(LSpot_4->m_pModelRootObject->m_pMesh);

	CLoadedModelInfo *Grass = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/Grass.bin", NULL, false);
	m_ppPlayGroundObjects[31] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[31]->SetChild(Grass->m_pModelRootObject, true);
	m_ppPlayGroundObjects[31]->SetPosition(-100.0f, -2000.f, -550.0f);
	m_ppPlayGroundObjects[31]->SetScale(9.5f, 19.5f, 9.5f);
	m_ppPlayGroundObjects[31]->SetMesh(Grass->m_pModelRootObject->m_pMesh);

	/////
	CLoadedModelInfo *CenterSpot_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/spot.bin", NULL, false);
	m_ppPlayGroundObjects[32] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[32]->SetChild(CenterSpot_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[32]->SetPosition(1845.0f, 80.f, 1294.0f);
	m_ppPlayGroundObjects[32]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[32]->SetMesh(CenterSpot_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[32]->SetOOBB(XMFLOAT3(1845.0f, 60.f, 1294.0f), XMFLOAT3(m_ppPlayGroundObjects[32]->m_pMesh->GetAABBExtents().x * objScale * 20, 45, m_ppPlayGroundObjects[32]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[32]->Rotate(0.0f, -90.0f, 0.0f);

	CLoadedModelInfo *CenterSpot_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/spot.bin", NULL, false);
	m_ppPlayGroundObjects[33] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[33]->SetChild(CenterSpot_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[33]->SetPosition(1160.0f, 80.f, 1294.0f);
	m_ppPlayGroundObjects[33]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[33]->SetMesh(CenterSpot_2->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[33]->SetOOBB(XMFLOAT3(1160.0f, 60.f, 1294.0f), XMFLOAT3(m_ppPlayGroundObjects[33]->m_pMesh->GetAABBExtents().x * objScale * 23, 45, m_ppPlayGroundObjects[33]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[33]->Rotate(0.0f, 90.0f, 0.0f);

	CLoadedModelInfo *CenterSpot_3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/spot.bin", NULL, false);
	m_ppPlayGroundObjects[34] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[34]->SetChild(CenterSpot_3->m_pModelRootObject, true);
	m_ppPlayGroundObjects[34]->SetPosition(1160.0f, 80, 708.0f);
	m_ppPlayGroundObjects[34]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[34]->SetMesh(CenterSpot_3->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[34]->SetOOBB(XMFLOAT3(1160.0f, 60, 708.0f), XMFLOAT3(m_ppPlayGroundObjects[34]->m_pMesh->GetAABBExtents().x * objScale * 23, 45, m_ppPlayGroundObjects[34]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[34]->Rotate(0.0f, 90.0f, 0.0f);

	CLoadedModelInfo *CenterSpot_4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/spot.bin", NULL, false);
	m_ppPlayGroundObjects[35] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[35]->SetChild(CenterSpot_4->m_pModelRootObject, true);
	m_ppPlayGroundObjects[35]->SetPosition(1845.0f, 80, 708.0f);
	m_ppPlayGroundObjects[35]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[35]->SetMesh(CenterSpot_4->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[35]->SetOOBB(XMFLOAT3(1845.0f, 60, 708.0f), XMFLOAT3(m_ppPlayGroundObjects[35]->m_pMesh->GetAABBExtents().x * objScale * 20, 45, m_ppPlayGroundObjects[35]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[35]->Rotate(0.0f, -90.0f, 0.0f);

	CLoadedModelInfo *St_Spot_1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/spot.bin", NULL, false);
	m_ppPlayGroundObjects[36] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[36]->SetChild(St_Spot_1->m_pModelRootObject, true);
	m_ppPlayGroundObjects[36]->SetPosition(1780.0f, 50, 300.0f);
	m_ppPlayGroundObjects[36]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[36]->SetMesh(St_Spot_1->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[36]->SetOOBB(XMFLOAT3(1780.0f, 50, 300.0f), XMFLOAT3(m_ppPlayGroundObjects[36]->m_pMesh->GetAABBExtents().x * objScale * 20, 25, m_ppPlayGroundObjects[36]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[36]->Rotate(0.0f, 180.0f, 0.0f);

	CLoadedModelInfo *St_Spot_2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê1/spot.bin", NULL, false);
	m_ppPlayGroundObjects[37] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppPlayGroundObjects[37]->SetChild(St_Spot_2->m_pModelRootObject, true);
	m_ppPlayGroundObjects[37]->SetPosition(1225.0f, 50, 300.0f);
	m_ppPlayGroundObjects[37]->SetScale(15.0f, 15.0f, 15.0f);
	m_ppPlayGroundObjects[37]->SetMesh(St_Spot_2->m_pModelRootObject->m_pMesh);
	m_ppPlayGroundObjects[37]->SetOOBB(XMFLOAT3(1225.0f, 50, 300.0f), XMFLOAT3(m_ppPlayGroundObjects[37]->m_pMesh->GetAABBExtents().x * objScale * 20, 25, m_ppPlayGroundObjects[37]->m_pMesh->GetAABBExtents().z * objScale * 20), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppPlayGroundObjects[37]->Rotate(0.0f, 180.0f, 0.0f);

	

	m_nConcertObjects = 50;
	m_ppConcertObjects = new CGameObject*[m_nConcertObjects];

	CLoadedModelInfo *BackGround = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ALL_PIECES.bin", NULL, false);
	m_ppConcertObjects[0] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[0]->SetChild(BackGround->m_pModelRootObject, true);
	m_ppConcertObjects[0]->SetPosition(1500.0f, 350, 1000.0f);
	m_ppConcertObjects[0]->SetScale(250, 250, 250);
	m_ppConcertObjects[0]->SetMesh(BackGround->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[0]->SetOOBB(m_ppConcertObjects[0]->GetPosition(), Vector3::ScalarProduct(m_ppConcertObjects[0]->m_pMesh->GetAABBExtents(), 150 * PG_objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * bottom = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/bottom.bin", NULL, false);
	m_ppConcertObjects[1] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[1]->SetChild(bottom->m_pModelRootObject, true);
	m_ppConcertObjects[1]->SetPosition(1500.0f, -40, 1000.0f);
	m_ppConcertObjects[1]->SetScale(250, 250, 250);
	m_ppConcertObjects[1]->SetMesh(bottom->m_pModelRootObject->m_pMesh);
	//m_ppConcertObjects[1]->SetOOBB(m_ppConcertObjects[1]->GetPosition(), Vector3::ScalarProduct(m_ppConcertObjects[1]->m_pMesh->GetAABBExtents(), 150 * PG_objScale), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * bluebottom = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/blue_bottom2.bin", NULL, false);
	m_ppConcertObjects[2] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[2]->SetChild(bluebottom->m_pModelRootObject, true);
	m_ppConcertObjects[2]->SetScale(250, 250, 250);
	m_ppConcertObjects[2]->SetMesh(bluebottom->m_pModelRootObject->m_pMesh);
	//m_ppConcertObjects[2]->SetOOBB(XMFLOAT3(1500.0f, 40, -1100.0f), XMFLOAT3(m_ppConcertObjects[2]->m_pMesh->GetAABBExtents().x * CONCERTScale, 35, m_ppConcertObjects[2]->m_pMesh->GetAABBExtents().z * CONCERTScale*16), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * bluerespawn = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/blue_bottom1.bin", NULL, false);
	m_ppConcertObjects[3] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[3]->SetChild(bluerespawn->m_pModelRootObject, true);
	m_ppConcertObjects[3]->SetScale(250, 250, 250);
	m_ppConcertObjects[3]->SetMesh(bluerespawn->m_pModelRootObject->m_pMesh);
	//m_ppConcertObjects[3]->SetOOBB(XMFLOAT3(1500.0f, 30, -940.0f), XMFLOAT3(m_ppConcertObjects[3]->m_pMesh->GetAABBExtents().x * CONCERTScale, 35, m_ppConcertObjects[3]->m_pMesh->GetAABBExtents().z * CONCERTScale*16), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * B_Pillar1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/¸Ê2±âµÕ.bin", NULL, false);
	m_ppConcertObjects[4] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[4]->SetChild(B_Pillar1->m_pModelRootObject, true);
	m_ppConcertObjects[4]->SetScale(250, 250, 250);
	m_ppConcertObjects[4]->SetMesh(B_Pillar1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[4]->SetOOBB(XMFLOAT3(2150.0f, 20, -470.0f), XMFLOAT3(m_ppConcertObjects[4]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[4]->m_pMesh->GetAABBExtents().z * 25), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * B_Pillar2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/¸Ê2±âµÕ.bin", NULL, false);
	m_ppConcertObjects[5] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[5]->SetChild(B_Pillar2->m_pModelRootObject, true);
	m_ppConcertObjects[5]->SetScale(250, 250, 250);
	m_ppConcertObjects[5]->SetMesh(B_Pillar2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[5]->SetOOBB(XMFLOAT3(850.0f, 20, -470.0f), XMFLOAT3(m_ppConcertObjects[4]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[4]->m_pMesh->GetAABBExtents().z * 25), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * B_Front_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·ç½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[6] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[6]->SetChild(B_Front_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[6]->SetScale(250, 250, 250);
	m_ppConcertObjects[6]->SetMesh(B_Front_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[6]->SetOOBB(XMFLOAT3(850.0f, 30, -350.0f), XMFLOAT3(m_ppConcertObjects[6]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[6]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * B_Front_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·ç½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[7] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[7]->SetChild(B_Front_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[7]->SetScale(250, 250, 250);
	m_ppConcertObjects[7]->SetMesh(B_Front_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[7]->SetOOBB(XMFLOAT3(2150.0f, 150, -350.0f), XMFLOAT3(m_ppConcertObjects[7]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[7]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Stage_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[8] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[8]->SetChild(C_B_Stage_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[8]->SetScale(250, 250, 250);
	m_ppConcertObjects[8]->SetMesh(C_B_Stage_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[8]->SetOOBB(XMFLOAT3(2020.0f, 180, -1060.0f), XMFLOAT3(m_ppConcertObjects[8]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[8]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Stage_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[9] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[9]->SetChild(C_B_Stage_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[9]->SetScale(250, 250, 250);
	m_ppConcertObjects[9]->SetMesh(C_B_Stage_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[9]->SetOOBB(XMFLOAT3(1880.0f, 180, -1110.0f), XMFLOAT3(m_ppConcertObjects[9]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[9]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Stage_Speaker3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[10] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[10]->SetChild(C_B_Stage_Speaker3->m_pModelRootObject, true);
	m_ppConcertObjects[10]->SetScale(250, 250, 250);
	m_ppConcertObjects[10]->SetMesh(C_B_Stage_Speaker3->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[10]->SetOOBB(XMFLOAT3(980.0f, 180, -1060.0f), XMFLOAT3(m_ppConcertObjects[10]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[10]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Stage_Speaker4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[11] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[11]->SetChild(C_B_Stage_Speaker4->m_pModelRootObject, true);
	m_ppConcertObjects[11]->SetScale(250, 250, 250);
	m_ppConcertObjects[11]->SetMesh(C_B_Stage_Speaker4->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[11]->SetOOBB(XMFLOAT3(1120.0f, 180, -1110.0f), XMFLOAT3(m_ppConcertObjects[11]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[11]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


	//RED
	CLoadedModelInfo * redbottom = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/blue_bottom2.bin", NULL, false);
	m_ppConcertObjects[12] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[12]->SetChild(redbottom->m_pModelRootObject, true);
	m_ppConcertObjects[12]->SetScale(250, 250, 250);
	m_ppConcertObjects[12]->SetMesh(redbottom->m_pModelRootObject->m_pMesh);
	//m_ppConcertObjects[12]->SetOOBB(XMFLOAT3(1500.0f, 40, 3100.0f), XMFLOAT3(m_ppConcertObjects[12]->m_pMesh->GetAABBExtents().x * CONCERTScale, 35, m_ppConcertObjects[12]->m_pMesh->GetAABBExtents().z * CONCERTScale*16), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * redrespawn = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/blue_bottom1.bin", NULL, false);
	m_ppConcertObjects[13] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[13]->SetChild(bluerespawn->m_pModelRootObject, true);
	m_ppConcertObjects[13]->SetScale(250, 250, 250);
	m_ppConcertObjects[13]->SetMesh(redrespawn->m_pModelRootObject->m_pMesh);
	//m_ppConcertObjects[13]->SetOOBB(XMFLOAT3(1500.0f, 30, 2940.0f), XMFLOAT3(m_ppConcertObjects[13]->m_pMesh->GetAABBExtents().x * CONCERTScale, 35, m_ppConcertObjects[13]->m_pMesh->GetAABBExtents().z * CONCERTScale*16), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * R_Pillar1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/¸Ê2±âµÕ.bin", NULL, false);
	m_ppConcertObjects[14] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[14]->SetChild(R_Pillar1->m_pModelRootObject, true);
	m_ppConcertObjects[14]->SetScale(250, 250, 250);
	m_ppConcertObjects[14]->SetMesh(R_Pillar1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[14]->SetOOBB(XMFLOAT3(2150.0f, 20, 2470.0f), XMFLOAT3(m_ppConcertObjects[14]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[14]->m_pMesh->GetAABBExtents().z * 25), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * R_Pillar2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/¸Ê2±âµÕ.bin", NULL, false);
	m_ppConcertObjects[15] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[15]->SetChild(R_Pillar2->m_pModelRootObject, true);
	m_ppConcertObjects[15]->SetScale(250, 250, 250);
	m_ppConcertObjects[15]->SetMesh(R_Pillar2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[15]->SetOOBB(XMFLOAT3(850.0f, 20, 2470.0f), XMFLOAT3(m_ppConcertObjects[14]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[14]->m_pMesh->GetAABBExtents().z * 25), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * R_Front_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·ç½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[16] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[16]->SetChild(R_Front_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[16]->SetScale(250, 250, 250);
	m_ppConcertObjects[16]->SetMesh(R_Front_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[16]->SetOOBB(XMFLOAT3(850.0f, 30, 2350.0f), XMFLOAT3(m_ppConcertObjects[16]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[16]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * R_Front_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·ç½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[17] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[17]->SetChild(R_Front_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[17]->SetScale(250, 250, 250);
	m_ppConcertObjects[17]->SetMesh(R_Front_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[17]->SetOOBB(XMFLOAT3(2150.0f, 150, 2350.0f), XMFLOAT3(m_ppConcertObjects[17]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[17]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Stage_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[18] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[18]->SetChild(C_R_Stage_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[18]->SetScale(250, 250, 250);
	m_ppConcertObjects[18]->SetMesh(C_R_Stage_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[18]->SetOOBB(XMFLOAT3(2020.0f, 180, 3060.0f), XMFLOAT3(m_ppConcertObjects[18]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[18]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Stage_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[19] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[19]->SetChild(C_R_Stage_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[19]->SetScale(250, 250, 250);
	m_ppConcertObjects[19]->SetMesh(C_R_Stage_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[19]->SetOOBB(XMFLOAT3(1880.0f, 180, 3110.0f), XMFLOAT3(m_ppConcertObjects[19]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[19]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Stage_Speaker3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[20] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[20]->SetChild(C_R_Stage_Speaker3->m_pModelRootObject, true);
	m_ppConcertObjects[20]->SetScale(250, 250, 250);
	m_ppConcertObjects[20]->SetMesh(C_R_Stage_Speaker3->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[20]->SetOOBB(XMFLOAT3(980.0f, 180, 3060.0f), XMFLOAT3(m_ppConcertObjects[20]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[20]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Stage_Speaker4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[21] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[21]->SetChild(C_R_Stage_Speaker4->m_pModelRootObject, true);
	m_ppConcertObjects[21]->SetScale(250, 250, 250);
	m_ppConcertObjects[21]->SetMesh(C_R_Stage_Speaker4->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[21]->SetOOBB(XMFLOAT3(1120.0f, 180, 3110.0f), XMFLOAT3(m_ppConcertObjects[21]->m_pMesh->GetAABBExtents().x * 280, 200, m_ppConcertObjects[21]->m_pMesh->GetAABBExtents().z * 160), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


	// RED ÀÛÀº ½ºÇÇÄ¿µé
	CLoadedModelInfo * C_R_Small_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[22] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[22]->SetChild(C_R_Small_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[22]->SetScale(250, 250, 250);
	m_ppConcertObjects[22]->SetMesh(C_R_Small_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[22]->SetOOBB(XMFLOAT3(2020.0f, 30, 2670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[23] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[23]->SetChild(C_R_Small_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[23]->SetScale(250, 250, 250);
	m_ppConcertObjects[23]->SetMesh(C_R_Small_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[23]->SetOOBB(XMFLOAT3(1740.0f, 30, 2670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[24] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[24]->SetChild(C_R_Small_Speaker3->m_pModelRootObject, true);
	m_ppConcertObjects[24]->SetScale(250, 250, 250);
	m_ppConcertObjects[24]->SetMesh(C_R_Small_Speaker3->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[24]->SetOOBB(XMFLOAT3(980.0f, 30, 2670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[25] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[25]->SetChild(C_R_Small_Speaker4->m_pModelRootObject, true);
	m_ppConcertObjects[25]->SetScale(250, 250, 250);
	m_ppConcertObjects[25]->SetMesh(C_R_Small_Speaker4->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[25]->SetOOBB(XMFLOAT3(1260.0f, 30, 2670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker5 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[26] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[26]->SetChild(C_R_Small_Speaker5->m_pModelRootObject, true);
	m_ppConcertObjects[26]->SetScale(250, 250, 250);
	m_ppConcertObjects[26]->SetMesh(C_R_Small_Speaker5->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[26]->SetOOBB(XMFLOAT3(1770.0f, 10, 1900.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker6 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[27] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[27]->SetChild(C_R_Small_Speaker6->m_pModelRootObject, true);
	m_ppConcertObjects[27]->SetScale(250, 250, 250);
	m_ppConcertObjects[27]->SetMesh(C_R_Small_Speaker6->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[27]->SetOOBB(XMFLOAT3(1770.0f, 10, 2220.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker7 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[28] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[28]->SetChild(C_R_Small_Speaker7->m_pModelRootObject, true);
	m_ppConcertObjects[28]->SetScale(250, 250, 250);
	m_ppConcertObjects[28]->SetMesh(C_R_Small_Speaker7->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[28]->SetOOBB(XMFLOAT3(1230.0f, 10, 1900.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_R_Small_Speaker8 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[29] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[29]->SetChild(C_R_Small_Speaker8->m_pModelRootObject, true);
	m_ppConcertObjects[29]->SetScale(250, 250, 250);
	m_ppConcertObjects[29]->SetMesh(C_R_Small_Speaker8->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[29]->SetOOBB(XMFLOAT3(1230.0f, 10, 2220.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


	//BLUE ÀÛÀº ½ºÇÇÄ¿µé
	CLoadedModelInfo * C_B_Small_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[30] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[30]->SetChild(C_B_Small_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[30]->SetScale(250, 250, 250);
	m_ppConcertObjects[30]->SetMesh(C_B_Small_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[30]->SetOOBB(XMFLOAT3(2020.0f, 30, -670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[31] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[31]->SetChild(C_B_Small_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[31]->SetScale(250, 250, 250);
	m_ppConcertObjects[31]->SetMesh(C_B_Small_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[31]->SetOOBB(XMFLOAT3(1740.0f, 30, -670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[32] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[32]->SetChild(C_B_Small_Speaker3->m_pModelRootObject, true);
	m_ppConcertObjects[32]->SetScale(250, 250, 250);
	m_ppConcertObjects[32]->SetMesh(C_B_Small_Speaker3->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[32]->SetOOBB(XMFLOAT3(980.0f, 30, -670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[33] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[33]->SetChild(C_B_Small_Speaker4->m_pModelRootObject, true);
	m_ppConcertObjects[33]->SetScale(250, 250, 250);
	m_ppConcertObjects[33]->SetMesh(C_B_Small_Speaker4->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[33]->SetOOBB(XMFLOAT3(1260.0f, 30, -670.0f), XMFLOAT3(70, 50, 30), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker5 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[34] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[34]->SetChild(C_B_Small_Speaker5->m_pModelRootObject, true);
	m_ppConcertObjects[34]->SetScale(250, 250, 250);
	m_ppConcertObjects[34]->SetMesh(C_B_Small_Speaker5->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[34]->SetOOBB(XMFLOAT3(1770.0f, 10, 100.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker6 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[35] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[35]->SetChild(C_B_Small_Speaker6->m_pModelRootObject, true);
	m_ppConcertObjects[35]->SetScale(250, 250, 250);
	m_ppConcertObjects[35]->SetMesh(C_B_Small_Speaker6->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[35]->SetOOBB(XMFLOAT3(1770.0f, 10, -220.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker7 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[36] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[36]->SetChild(C_B_Small_Speaker7->m_pModelRootObject, true);
	m_ppConcertObjects[36]->SetScale(250, 250, 250);
	m_ppConcertObjects[36]->SetMesh(C_B_Small_Speaker7->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[36]->SetOOBB(XMFLOAT3(1230.0f, 10, 100.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_B_Small_Speaker8 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[37] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[37]->SetChild(C_B_Small_Speaker8->m_pModelRootObject, true);
	m_ppConcertObjects[37]->SetScale(250, 250, 250);
	m_ppConcertObjects[37]->SetMesh(C_B_Small_Speaker8->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[37]->SetOOBB(XMFLOAT3(1230.0f, 10, -220.0f), XMFLOAT3(30, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


	//°¡¿îµ¥ ½ºÇÇÄ¿µé
	CLoadedModelInfo * C_M_Small_Speaker1 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[38] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[38]->SetChild(C_M_Small_Speaker1->m_pModelRootObject, true);
	m_ppConcertObjects[38]->SetScale(250, 250, 250);
	m_ppConcertObjects[38]->SetMesh(C_M_Small_Speaker1->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[38]->SetOOBB(XMFLOAT3(1860.0f, 10, 1530.0f), XMFLOAT3(70, 35, 50), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker2 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[39] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[39]->SetChild(C_M_Small_Speaker2->m_pModelRootObject, true);
	m_ppConcertObjects[39]->SetScale(250, 250, 250);
	m_ppConcertObjects[39]->SetMesh(C_M_Small_Speaker2->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[39]->SetOOBB(XMFLOAT3(2060.0f, 10, 1360.0f), XMFLOAT3(60, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker3 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[40] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[40]->SetChild(C_M_Small_Speaker3->m_pModelRootObject, true);
	m_ppConcertObjects[40]->SetScale(250, 250, 250);
	m_ppConcertObjects[40]->SetMesh(C_M_Small_Speaker3->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[40]->SetOOBB(XMFLOAT3(2150.0f, 10, 1150.0f), XMFLOAT3(30, 35, 70), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker4 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[41] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[41]->SetChild(C_M_Small_Speaker4->m_pModelRootObject, true);
	m_ppConcertObjects[41]->SetScale(250, 250, 250);
	m_ppConcertObjects[41]->SetMesh(C_M_Small_Speaker4->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[41]->SetOOBB(XMFLOAT3(1140.0f, 10, 1530.0f), XMFLOAT3(70, 35, 50), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker5 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[42] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[42]->SetChild(C_M_Small_Speaker5->m_pModelRootObject, true);
	m_ppConcertObjects[42]->SetScale(250, 250, 250);
	m_ppConcertObjects[42]->SetMesh(C_M_Small_Speaker5->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[42]->SetOOBB(XMFLOAT3(940.0f, 10, 1360.0f), XMFLOAT3(60, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker6 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[43] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[43]->SetChild(C_M_Small_Speaker6->m_pModelRootObject, true);
	m_ppConcertObjects[43]->SetScale(250, 250, 250);
	m_ppConcertObjects[43]->SetMesh(C_M_Small_Speaker6->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[43]->SetOOBB(XMFLOAT3(850.0f, 10, 1150.0f), XMFLOAT3(30, 35, 70), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker7 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[44] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[44]->SetChild(C_M_Small_Speaker7->m_pModelRootObject, true);
	m_ppConcertObjects[44]->SetScale(250, 250, 250);
	m_ppConcertObjects[44]->SetMesh(C_M_Small_Speaker7->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[44]->SetOOBB(XMFLOAT3(1860.0f, 10, 470.0f), XMFLOAT3(70, 35, 50), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker8 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[45] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[45]->SetChild(C_M_Small_Speaker8->m_pModelRootObject, true);
	m_ppConcertObjects[45]->SetScale(250, 250, 250);
	m_ppConcertObjects[45]->SetMesh(C_M_Small_Speaker8->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[45]->SetOOBB(XMFLOAT3(2060.0f, 10, 640.0f), XMFLOAT3(60, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker9 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[46] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[46]->SetChild(C_M_Small_Speaker9->m_pModelRootObject, true);
	m_ppConcertObjects[46]->SetScale(250, 250, 250);
	m_ppConcertObjects[46]->SetMesh(C_M_Small_Speaker9->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[46]->SetOOBB(XMFLOAT3(2150.0f, 10, 850.0f), XMFLOAT3(30, 35, 70), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker10 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[47] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[47]->SetChild(C_M_Small_Speaker10->m_pModelRootObject, true);
	m_ppConcertObjects[47]->SetScale(250, 250, 250);
	m_ppConcertObjects[47]->SetMesh(C_M_Small_Speaker4->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[47]->SetOOBB(XMFLOAT3(1140.0f, 10, 470.0f), XMFLOAT3(70, 35, 50), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker11 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[48] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[48]->SetChild(C_M_Small_Speaker11->m_pModelRootObject, true);
	m_ppConcertObjects[48]->SetScale(250, 250, 250);
	m_ppConcertObjects[48]->SetMesh(C_M_Small_Speaker11->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[48]->SetOOBB(XMFLOAT3(940.0f, 10, 640.0f), XMFLOAT3(60, 35, 60), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CLoadedModelInfo * C_M_Small_Speaker12 = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/¸Ê2/ºí·çÁ÷°¢½ºÇÇÄ¿.bin", NULL, false);
	m_ppConcertObjects[49] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_ppConcertObjects[49]->SetChild(C_M_Small_Speaker12->m_pModelRootObject, true);
	m_ppConcertObjects[49]->SetScale(250, 250, 250);
	m_ppConcertObjects[49]->SetMesh(C_M_Small_Speaker12->m_pModelRootObject->m_pMesh);
	m_ppConcertObjects[49]->SetOOBB(XMFLOAT3(850.0f, 10, 850.0f), XMFLOAT3(30, 35, 70), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//if (Floor) delete Floor;
	if (Floor_Slide) delete Floor_Slide;
	if (Floor_Stage) delete Floor_Stage;
	if (Slide) delete Slide;
	if (Respawn_Blue) delete Respawn_Blue;
	if (Respawn_Red) delete Respawn_Red;

	
	if (LeftSeeSaw) delete LeftSeeSaw;
	if (RightSeeSaw) delete RightSeeSaw;


	if (Slide_Speaker_3) delete Slide_Speaker_3;
	if (Slide_Speaker_1) delete Slide_Speaker_1;
	if (Stage) delete Stage;
	if (LeftSpring_1) delete LeftSpring_1;
	if (Left_Spring_2) delete Left_Spring_2;
	if (Right_Spring_1) delete Right_Spring_1;
	if (RightSpring_2) delete RightSpring_2;

	if (Chair_1) delete Chair_1;
	if (Chair_2) delete Chair_2;
	if (Street_Lamp_1) delete Street_Lamp_1;
	if (Street_Lamp_2) delete Street_Lamp_2;
	if (Stage_Speaker_1) delete Stage_Speaker_1;
	if (Stage_Speaker_2) delete Stage_Speaker_2;
	if (Stage_Speaker_3) delete Stage_Speaker_3;
	if (Stage_Speaker_4) delete Stage_Speaker_4;

	if (RSpot_1) delete RSpot_1;
	if (RSpot_2) delete RSpot_2;
	if (RSpot_3) delete RSpot_3;
	if (RSpot_4) delete RSpot_4;

	if (LSpot_1) delete LSpot_1;
	if (LSpot_2) delete LSpot_2;
	if (LSpot_3) delete LSpot_3;
	if (LSpot_4) delete LSpot_4;

	if (Grass) delete Grass;

	if (CenterSpot_1) delete CenterSpot_1;
	if (CenterSpot_2) delete CenterSpot_2;
	if (CenterSpot_3) delete CenterSpot_3;
	if (CenterSpot_4) delete CenterSpot_4;
	if (St_Spot_1) delete St_Spot_1;
	if (St_Spot_2) delete St_Spot_2;


	//¸Ê2

	if (BackGround) delete BackGround;
	if (bottom) delete bottom;
	if (bluerespawn) delete bluerespawn;
	if (B_Pillar1) delete B_Pillar1;
	if (B_Pillar2) delete B_Pillar2;
	if (B_Front_Speaker1) delete B_Front_Speaker1;
	if (B_Front_Speaker2) delete B_Front_Speaker2;
	if (C_B_Stage_Speaker1) delete C_B_Stage_Speaker1;
	if (C_B_Stage_Speaker2) delete C_B_Stage_Speaker2;
	if (C_B_Stage_Speaker3) delete C_B_Stage_Speaker3;
	if (C_B_Stage_Speaker4) delete C_B_Stage_Speaker4;
	if (redbottom) delete redbottom;
	if (redrespawn) delete redrespawn;
	if (R_Pillar1) delete R_Pillar1;
	if (R_Pillar2) delete R_Pillar2;
	if (R_Front_Speaker1) delete R_Front_Speaker1;
	if (R_Front_Speaker2) delete R_Front_Speaker2;
	if (C_R_Stage_Speaker1) delete C_R_Stage_Speaker1;
	if (C_R_Stage_Speaker2) delete C_R_Stage_Speaker2;
	if (C_R_Stage_Speaker3) delete C_R_Stage_Speaker3;
	if (C_R_Stage_Speaker4) delete C_R_Stage_Speaker4;
	if (C_R_Small_Speaker1) delete C_R_Small_Speaker1;
	if (C_R_Small_Speaker2) delete C_R_Small_Speaker2;
	if (C_R_Small_Speaker3) delete C_R_Small_Speaker3;
	if (C_R_Small_Speaker4) delete C_R_Small_Speaker4;
	if (C_R_Small_Speaker5) delete C_R_Small_Speaker5;
	if (C_R_Small_Speaker6) delete C_R_Small_Speaker6;
	if (C_R_Small_Speaker7) delete C_R_Small_Speaker7;
	if (C_R_Small_Speaker8) delete C_R_Small_Speaker8;
	if (C_B_Small_Speaker1) delete C_B_Small_Speaker1;
	if (C_B_Small_Speaker2) delete C_B_Small_Speaker2;
	if (C_B_Small_Speaker3) delete C_B_Small_Speaker3;
	if (C_B_Small_Speaker4) delete C_B_Small_Speaker4;
	if (C_B_Small_Speaker5) delete C_B_Small_Speaker5;
	if (C_B_Small_Speaker6) delete C_B_Small_Speaker6;
	if (C_B_Small_Speaker7) delete C_B_Small_Speaker7;
	if (C_B_Small_Speaker8) delete C_B_Small_Speaker8;
	if (C_M_Small_Speaker1) delete C_M_Small_Speaker1;
	if (C_M_Small_Speaker2) delete C_M_Small_Speaker2;
	if (C_M_Small_Speaker3) delete C_M_Small_Speaker3;
	if (C_M_Small_Speaker4) delete C_M_Small_Speaker4;
	if (C_M_Small_Speaker5) delete C_M_Small_Speaker5;
	if (C_M_Small_Speaker6) delete C_M_Small_Speaker6;
	if (C_M_Small_Speaker7) delete C_M_Small_Speaker7;
	if (C_M_Small_Speaker8) delete C_M_Small_Speaker8;
	if (C_M_Small_Speaker9) delete C_M_Small_Speaker9;
	if (C_M_Small_Speaker10) delete C_M_Small_Speaker10;
	if (C_M_Small_Speaker11) delete C_M_Small_Speaker11;
	if (C_M_Small_Speaker12) delete C_M_Small_Speaker12;

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
	
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[13];

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
	pd3dDescriptorRanges[6].BaseShaderRegister = 12; //t12: gtxtEmissionTexture to Shadow
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
	pd3dDescriptorRanges[10].BaseShaderRegister = 14; //¿©±â
	pd3dDescriptorRanges[10].RegisterSpace = 0;
	pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[11].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[11].NumDescriptors = 1;
	pd3dDescriptorRanges[11].BaseShaderRegister = 15; //t15 
	pd3dDescriptorRanges[11].RegisterSpace = 0;
	pd3dDescriptorRanges[11].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[12].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[12].NumDescriptors = 1;
	pd3dDescriptorRanges[12].BaseShaderRegister = 16; //t15 //¸Þ´º¾À
	pd3dDescriptorRanges[12].RegisterSpace = 0;
	pd3dDescriptorRanges[12].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	//pd3dDescriptorRanges[10].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[10].NumDescriptors = 1;						//m_nScreenOffRederTargetÀÇ °³¼ö
	//pd3dDescriptorRanges[10].BaseShaderRegister = 20; //T20: gtxtScene
	//pd3dDescriptorRanges[10].RegisterSpace = 0;
	//pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	D3D12_ROOT_PARAMETER pd3dRootParameters[20];

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

	pd3dRootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[9].Descriptor.ShaderRegister = 14; //HUN1
	pd3dRootParameters[9].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[10].Descriptor.ShaderRegister = 13; //Sprite
	pd3dRootParameters[10].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	
	pd3dRootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[11].Descriptor.ShaderRegister = 7; //Skinned Bone Offsets
	pd3dRootParameters[11].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[12].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[12].Descriptor.ShaderRegister = 8; //Skinned Bone Transforms
	pd3dRootParameters[12].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[12].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[13].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[13].Descriptor.ShaderRegister = 12; //Clock
	pd3dRootParameters[13].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[13].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[14].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[14].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[14].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[10]);
	pd3dRootParameters[14].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //¿©±ë

	pd3dRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[15].Descriptor.ShaderRegister = 5;
	pd3dRootParameters[15].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[16].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[16].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[16].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[11]);
	pd3dRootParameters[16].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[17].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[17].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[17].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[12]);
	pd3dRootParameters[17].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	pd3dRootParameters[18].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[18].Descriptor.ShaderRegister = 9; //HP
	pd3dRootParameters[18].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[18].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[19].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[19].Descriptor.ShaderRegister = 10; //Skill Cool
	pd3dRootParameters[19].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[19].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//pd3dRootParameters[20].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pd3dRootParameters[20].Descriptor.ShaderRegister = 11; 
	//pd3dRootParameters[20].Descriptor.RegisterSpace = 0;
	//pd3dRootParameters[20].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	
	//pd3dRootParameters[21].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pd3dRootParameters[21].Descriptor.ShaderRegister = 14; //SPRIT
	//pd3dRootParameters[21].Descriptor.RegisterSpace = 0;
	//pd3dRootParameters[21].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//

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
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256ÀÇ ¹è¼ö
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
		case VK_F8:
			m_WholeMapRender = !m_WholeMapRender;
			break;
		case 'W':
			//PLAYER->GetOtherPlayer()->SetPlayerState(RUN);
			break;
		case 'S':
			//m_ppPlayGroundObjects[0]->MoveForward(-3.0f);
			break;
		case 'A':
			//m_ppPlayGroundObjects[0]->MoveStrafe(-3.0f);
			break;
		case 'D':
			//m_ppPlayGroundObjects[0]->MoveStrafe(+3.0f);
			break;
		case 'Q':
			//m_ppPlayGroundObjects[0]->MoveUp(+3.0f);
			break;
		case 'R':
			//m_ppPlayGroundObjects[0]->MoveUp(-3.0f);
			break;
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

	switch (SCENEMANAGER->GetSceneType())
	{
	case MENUSCENE:

		if (m_ppShaders[2])
		{
			m_ppShaders[2]->Render(pd3dCommandList, pCamera);
		}

		break;
	case PLAYGROUNDMAP:

		for (int i = 0; i < m_nPlayGroundObjects; i++)
		{
			m_ppPlayGroundObjects[i]->Render(pd3dCommandList, pCamera);
		}
		
		for (int i = 3; i < 7; ++i)
		{
			if (i != 5)
			{

				if (m_ppShaders[i]) m_ppShaders[i]->Render(pd3dCommandList, pCamera);
			}
		}
		if (PLAYER->GetPlayer()->GetClientNum() == CNETWORK->myid) {
			if (PLAYER->GetPlayer()->GetPlayerState() == HAPPY) {
				if (m_ppShaders[8])
					m_ppShaders[8]->Render(pd3dCommandList, pCamera);
			}
			if (PLAYER->GetPlayer()->GetPlayerState() == SAD) {
				if (m_ppShaders[7])
					m_ppShaders[7]->Render(pd3dCommandList, pCamera);
			}
		}
		else {
			if (PLAYER->GetOtherPlayer()->GetPlayerState() == HAPPY) {
				if (m_ppShaders[8])
					m_ppShaders[8]->Render(pd3dCommandList, pCamera);
			}

			if (PLAYER->GetOtherPlayer()->GetPlayerState() == SAD) {
				if (m_ppShaders[7])
					m_ppShaders[7]->Render(pd3dCommandList, pCamera);
			}
		}
		if (m_ppShaders[9]) 
		{
			if (PLAYER->GetPlayer()->isHeartRender())
			{
				m_ppShaders[9]->UpdateShaderVariables(pd3dCommandList);
				m_ppShaders[9]->Render(pd3dCommandList, pCamera);
			}
	
		}
		for(int i =  11; i < m_nShaders;++i)
		{
			if (m_ppShaders[i])
			{
				m_ppShaders[i]->UpdateShaderVariables(pd3dCommandList);
				m_ppShaders[i]->Render(pd3dCommandList, pCamera);
			}
		}
		
		break;

	case CONCERTMAP:
		for (int i = 3; i < 7; ++i)
		{
			if (m_ppShaders[i]) m_ppShaders[i]->Render(pd3dCommandList, pCamera);
		}
		if (m_WholeMapRender)
		{
			if (m_ppConcertObjects[0])
			{
				m_ppConcertObjects[0]->Render(pd3dCommandList, pCamera);

			}
		}
		for (int i = 1; i <2; i++)
		{
			m_ppConcertObjects[i]->Render(pd3dCommandList, pCamera);
		}
		if (PLAYER->GetPlayer()->GetClientNum() == CNETWORK->myid) {
			if (PLAYER->GetPlayer()->GetPlayerState() == HAPPY) {
				if (m_ppShaders[8])
					m_ppShaders[8]->Render(pd3dCommandList, pCamera);
			}
			if (PLAYER->GetPlayer()->GetPlayerState() == SAD) {
				if (m_ppShaders[7])
					m_ppShaders[7]->Render(pd3dCommandList, pCamera);
			}
		}
		else {
			if (PLAYER->GetOtherPlayer()->GetPlayerState() == HAPPY) {
				if (m_ppShaders[8])
					m_ppShaders[8]->Render(pd3dCommandList, pCamera);
			}

			if (PLAYER->GetOtherPlayer()->GetPlayerState() == SAD) {
				if (m_ppShaders[7])
					m_ppShaders[7]->Render(pd3dCommandList, pCamera);
			}
		}
		if (m_ppShaders[9])
		{
			
			if (PLAYER->GetPlayer()->isHeartRender())
			{
				m_ppShaders[9]->UpdateShaderVariables(pd3dCommandList);
				m_ppShaders[9]->Render(pd3dCommandList, pCamera);
			}
		}
		for (int i = 11; i < m_nShaders; ++i)
		{
			if (m_ppShaders[i])
			{
				m_ppShaders[i]->UpdateShaderVariables(pd3dCommandList);
				m_ppShaders[i]->Render(pd3dCommandList, pCamera);
			}
		}
		break;
	}
}
void CScene::CheckObjectByObjectCollisions() 
{
	int num = 0;
	int num2 = 0;


	switch (SCENEMANAGER->GetSceneType())
	{
	case PLAYGROUNDMAP:
		for (int i = 0; i < m_nPlayGroundObjects; i++)
		{
			if (m_ppPlayGroundObjects[i])
			{

				if (m_ppPlayGroundObjects[i]->GetBoundingBox().Intersects(PLAYER->GetPlayer()->GetBoundingBox()))
				{
					if (m_ppPlayGroundObjects[i]->GetBoundingBox().Extents.y + m_ppPlayGroundObjects[i]->GetBoundingBox().Center.y <= PLAYER->GetPlayer()->GetBoundingBox().Center.y - PLAYER->GetPlayer()->GetBoundingBox().Extents.y + 10)
					{
						PLAYER->GetPlayer()->SetHeight(m_ppPlayGroundObjects[i]->GetBoundingBox().Extents.y + m_ppPlayGroundObjects[i]->GetBoundingBox().Center.y);
						PLAYER->GetPlayer()->SetPlayCrashMap(true);
						PLAYER->GetPlayer()->SetCollisionState(false);

						num++;
					}

				}
				if (m_ppPlayGroundObjects[i]->GetBoundingBox().Contains(PLAYER->GetPlayer()->GetBoundingBox()))
				{
					PLAYER->GetPlayer()->SetCollisionState(true);
				}

				if (m_ppPlayGroundObjects[i]->GetBoundingBox().Intersects(PLAYER->GetOtherPlayer()->GetBoundingBox()))
				{
					if (m_ppPlayGroundObjects[i]->GetBoundingBox().Extents.y + m_ppPlayGroundObjects[i]->GetBoundingBox().Center.y <= PLAYER->GetOtherPlayer()->GetBoundingBox().Center.y - PLAYER->GetOtherPlayer()->GetBoundingBox().Extents.y + 10)
					{
						PLAYER->GetOtherPlayer()->SetHeight(m_ppPlayGroundObjects[i]->GetBoundingBox().Extents.y + m_ppPlayGroundObjects[i]->GetBoundingBox().Center.y);
						PLAYER->GetOtherPlayer()->SetPlayCrashMap(true);
						PLAYER->GetOtherPlayer()->SetCollisionState(false);
						num2++;
					}
				}
				if (m_ppPlayGroundObjects[i]->GetBoundingBox().Contains(PLAYER->GetOtherPlayer()->GetBoundingBox()))
				{
					PLAYER->GetOtherPlayer()->SetCollisionState(true);
				}
			}
		}
		if (num == 0)
		{
			PLAYER->GetPlayer()->SetPlayCrashMap(false);
		}
		break;
	case CONCERTMAP:
		for (int i = 1; i < m_nConcertObjects; i++)
		{
			if (m_ppConcertObjects[i])
			{
				if (i == 1)
				{
					if (m_ppConcertObjects[1]->GetBoundingBox().Contains(PLAYER->GetPlayer()->GetBoundingBox()))
					{
						cout << "¹«´ë ¾È" << endl;
						PLAYER->GetPlayer()->SetHeight(m_ppConcertObjects[i]->GetBoundingBox().Extents.y + m_ppConcertObjects[i]->GetBoundingBox().Center.y);


					}
				}

				if (m_ppConcertObjects[i]->GetBoundingBox().Intersects(PLAYER->GetPlayer()->GetBoundingBox()))
				{
					if (m_ppConcertObjects[i]->GetBoundingBox().Extents.y + m_ppConcertObjects[i]->GetBoundingBox().Center.y <= PLAYER->GetPlayer()->GetBoundingBox().Center.y - PLAYER->GetPlayer()->GetBoundingBox().Extents.y + 10)
					{
						PLAYER->GetPlayer()->SetHeight(m_ppConcertObjects[i]->GetBoundingBox().Extents.y + m_ppConcertObjects[i]->GetBoundingBox().Center.y);
						cout << i << "¹øÂ° ¿ÉÁ§" << endl;
						PLAYER->GetPlayer()->SetPlayCrashMap(true);
						PLAYER->GetPlayer()->SetCollisionState(false);

						num++;
					}

				}
				if (m_ppConcertObjects[i]->GetBoundingBox().Contains(PLAYER->GetPlayer()->GetBoundingBox()))
				{
					PLAYER->GetPlayer()->SetCollisionState(true);
				}
			}
		}
		if (num == 0)
		{
			PLAYER->GetPlayer()->SetPlayCrashMap(false);

		}

	break;
	}


}
