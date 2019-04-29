#include "stdafx.h"
#include "CIngameScene.h"


CInGameScene::CInGameScene()
{
	m_SceneType = INGAME;
}

CInGameScene::~CInGameScene()
{
}

void CInGameScene::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CInGameScene::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CInGameScene::ReleaseShaderVariables()
{
}

bool CInGameScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CInGameScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CInGameScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	CScene::BuildObjects(pd3dDevice, pd3dCommandList);
	//CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()
	//CreateShaderResourceViews()
	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.3f, 0.0f, 0.0f);
	//m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Terrain/white.raw"), 257, 257, xmf3Scale, xmf4Color);


	//m_nShaders = 0;

	//CLoadedModelInfo *pMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALL_(1).bin", NULL, false);
	////	
	//CAngrybotObjectsShader *pPlayGroundObjectsShader = new CAngrybotObjectsShader();
	//pPlayGroundObjectsShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//pPlayGroundObjectsShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//pPlayGroundObjectsShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pTerrain);


	//CLoadedModelInfo *pPlayGroundMapObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ALL.bin", NULL, false);
	
	//

	//m_ppShaders = new CShader*[m_nShaders];
	//m_ppShaders[1] = pPlayGroundObjectsShader;
	//m_ppGameObjects[0]->SetPosition(200.0f, 151, 700.0f); //¸Ê °Å²Ù·Î ¹ö±× ¿©±â
	//m_ppGameObjects[0]->SetOOBB(m_ppGameObjects[0]->GetPosition(), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.f));
	//m_ppGameObjects[0]->SetScale(200.0f, 200.0f, 200.0f);

	////2¹ø¤Š ¸Ê
	//m_ppGameObjects[1] = new MapObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	//m_ppGameObjects[1]->SetChild(pMapObject->m_pModelRootObject, false);
	//m_ppGameObjects[1]->SetPosition(200.0f, m_pTerrain->GetHeight(400.0f, 700.0f), 700.0f);
	//m_ppGameObjects[1]->SetOOBB(m_ppGameObjects[1]->GetPosition(), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.f));
	//m_ppGameObjects[1]->SetScale(200.0f, 200.0f, 200.0f);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CInGameScene::ReleaseObjects()
{
}

void CInGameScene::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	//if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	//if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	CScene::Render(pd3dCommandList, pCamera);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	//D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	//pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	//if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);
	//m_nShaders = 0;
	//for (int i = 0; i < m_nShaders; i++) if (m_ppShaders[i]) m_ppShaders[i]->Render(pd3dCommandList, pCamera);

}

bool CInGameScene::ProcessInput(UCHAR * pKeysBuffer)
{
	return false;
}

//void CInGameScene::CreateCbvSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
//{
//	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
//	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
//	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	d3dDescriptorHeapDesc.NodeMask = 0;
//	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);
//
//	m_d3dCbvCPUDescriptorNextHandle = m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
//	m_d3dCbvGPUDescriptorNextHandle = m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
//	m_d3dSrvCPUDescriptorNextHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
//	m_d3dSrvGPUDescriptorNextHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
//
//}

//D3D12_GPU_DESCRIPTOR_HANDLE CInGameScene::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
//{
//	return D3D12_GPU_DESCRIPTOR_HANDLE();
//}
//
//D3D12_GPU_DESCRIPTOR_HANDLE CInGameScene::CreateShaderResourceViews(ID3D12Device * pd3dDevice, CTexture * pTexture, UINT nRootParameter, bool bAutoIncrement)
//{
//	return D3D12_GPU_DESCRIPTOR_HANDLE();
//}
