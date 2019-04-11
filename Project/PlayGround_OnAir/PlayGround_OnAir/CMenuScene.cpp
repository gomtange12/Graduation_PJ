#include "stdafx.h"
#include "CMenuScene.h"


CMenuScene::CMenuScene()
{
	m_nLights = 0;
	m_SceneType = INGAME;

}

CMenuScene::~CMenuScene()
{

}

void CMenuScene::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbMenuLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMenuLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CMenuScene::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CMenuScene::ReleaseShaderVariables()
{
}

bool CMenuScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMenuScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CMenuScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	//CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()
	//CreateShaderResourceViews()
	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_nShaders = 0;
	m_ppShaders = new CShader*[m_nShaders];


 	CreateShaderVariables(pd3dDevice, pd3dCommandList);

}

void CMenuScene::ReleaseObjects()
{
}

void CMenuScene::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
}

bool CMenuScene::ProcessInput(UCHAR * pKeysBuffer)
{
	return false;
}

void CMenuScene::CreateCbvSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
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

D3D12_GPU_DESCRIPTOR_HANDLE CMenuScene::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE CMenuScene::CreateShaderResourceViews(ID3D12Device * pd3dDevice, CTexture * pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

