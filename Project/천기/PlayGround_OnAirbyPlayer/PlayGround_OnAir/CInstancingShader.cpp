#include "stdafx.h"
#include "CInstancingShader.h"


CInstancingShader::CInstancingShader()
{
}

CInstancingShader::~CInstancingShader()
{
}
D3D12_DEPTH_STENCIL_DESC CInstancingShader::CreateDepthStencilState()
{
	//����
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	/*d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;*/

	return(d3dDepthStencilDesc);
}

D3D12_BLEND_DESC CInstancingShader::CreateBlendState()
{
	//����
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	/*d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;*/

	return(d3dBlendDesc);
}
D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//���� ������ ���� �Է� �����̴�.
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CInstancingShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
		ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CInstancingShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
		ppd3dShaderBlob));
}

void CInstancingShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	/*m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);*/
}

void CInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);

	//�ν��Ͻ� ������ ������ ���� ���۸� ���ε� �� �������� �����Ѵ�.
	//m_pd3dcbGrassObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
	//	sizeof(VS_VB_INSTANCE) * m_nGrass, D3D12_HEAP_TYPE_UPLOAD,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	////���� ����(���ε� ��)�� ���� �����͸� �����Ѵ�.
	//m_pd3dcbGrassObjects->Map(0, NULL, (void **)&m_pcbMappedGrassObjects);

	////��?�� �� �������� (���Ŀ� Ȯ�� �ٽ� �غ� ��)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbTreeObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nTree;
}

void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{


	pd3dCommandList->SetGraphicsRootDescriptorTable(m_pTexture->m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);

	//���̺�� �����ؼ� �Ѱܾ���. 
	for (int j = 0; j < m_nGrass; j++)
	{
		XMStoreFloat4x4(&m_pcbMappedUIObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppUI[j]->m_xmf4x4World)));
	}
	pd3dCommandList->SetGraphicsRootShaderResourceView(7, //���Ⱑ 2�ƴ϶� 3
		m_pd3dcbUIObjects->GetGPUVirtualAddress());
}

void CInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbUIObjects) m_pd3dcbUIObjects->Unmap(0, NULL);
	if (m_pd3dcbUIObjects) m_pd3dcbUIObjects->Release();
}

void CInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void *pHeightMap)
{
	
		//�Ȱ�
	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;

	//float fTerrainWidth = pTerrain->GetWidth();
	//float fTerrainLength = pTerrain->GetLength();

	////������Ʈ�� ������ ���� ����ü�� �򸮴°� �ٲ�
	//int xObjects = 160;
	//int zObjects = 160;
	//m_nGrass = (xObjects * zObjects);

	//CTexture *ppGrassTextures;
	//ppGrassTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppGrassTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass01.dds", 0);
	///*ppGrassTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppGrassTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass02.dds", 0);
	//CTexture *ppFlowerTextures[2];
	//ppFlowerTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppFlowerTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower01.dds", 0);
	//ppFlowerTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppFlowerTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower02.dds", 0);*/

	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	//CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // �ؽ��� srv
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
	////CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures, 3, true);
	////CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[1], 3, true);
	////CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[0], 3, true);
	////CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[1], 3, true); // �� ������ �ؽ��� �ϳ��� �׷����°� �翬�ϳ�... �ᱹ �길 �Ǵ����̾���


	//CMaterial *ppGrassMaterials;
	//ppGrassMaterials = new CMaterial();
	//ppGrassMaterials->SetTexture(ppGrassTextures);
	///*ppGrassMaterials[1] = new CMaterial();
	//ppGrassMaterials[1]->SetTexture(ppGrassTextures[1]);*/





	///*CMaterial *ppFlowerMaterials[2];

	//ppFlowerMaterials[0] = new CMaterial();
	//ppFlowerMaterials[0]->SetTexture(ppFlowerTextures[0]);
	//ppFlowerMaterials[1] = new CMaterial();
	//ppFlowerMaterials[1]->SetTexture(ppFlowerTextures[1]);*/


	//CTexturedRectMesh *pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 5.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	////CTexturedRectMesh *pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 5.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f); // �������� ũ�� ����

	//m_ppGrass = new CGameObject*[m_nGrass];

	//CGrassObject *pBillboardObject = NULL;

	////�򸮴� �е� ������
	//float fxPitch = 0.15f;
	//float fzPitch = 0.15f;

	//CMaterial *pMaterial = NULL;
	//CMesh *pMesh = NULL;

	//float xPosition = 0.0f; // �򸮴� ��ġ ����
	//for (int i = 0, x = 0; x < xObjects; x++)
	//{
	//	xPosition += x * fxPitch;
	//	float zPosition = 0.0f;
	//	for (int z = 0; z < zObjects; z++)
	//	{
	//		pBillboardObject = new CGrassObject();

	//		//pBillboardObject->SetMesh(0, pFlowerMesh);

	//		//if (x % 2) pMaterial = (z % 2) ? ppGrassMaterials[0] : ppFlowerMaterials[1];
	//		//else pMaterial = (z % 2) ? ppGrassMaterials[1] : ppFlowerMaterials[0]; // z�� 0�̸� �ö�� 1dds, z�� 1�̸� �׷��� 2dds�� ���͸��� ���Ѵ�.
	//		pBillboardObject->SetMaterial(ppGrassMaterials);

	//		zPosition += z * fzPitch;
	//		float fHeight = pTerrain->GetHeight(xPosition, zPosition);
	//		pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 4.0f : 8.0f) * 0.5f), zPosition); // ���������� ����Ʈ�� �����ؼ� �ٴڿ��� �ڶ���� ����. Ǯ ���������� �������ִ� �κ���
	//		pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
	//		m_ppGrass[i++] = pBillboardObject;

	//	}
	//}
	//m_ppGrass[0]->SetMesh(0, pGrassMesh);
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);//�̰� ���Ծ����

}

void CInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	//OnPrepareRender(pd3dCommandList);

	//CObjectsShader::Render(pd3dCommandList, pCamera);
	////��� ���� ��ü�� �ν��Ͻ� �����͸� ���ۿ� �����Ѵ�.
	//UpdateShaderVariables(pd3dCommandList);
	////CTexturedShader::Render(pd3dCommandList, pCamera);


	////�ϳ��� ���� �����͸� ����Ͽ� ��� ���� ��ü(�ν��Ͻ�)���� �������Ѵ�.
	//m_ppGrass[0]->Render(pd3dCommandList, pCamera, m_nGrass); //����
}

void CInstancingShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	/*for (int i = 0; i < m_nGrass; ++i)
	{
		m_ppGrass[i]->Animate(fTimeElapsed, pCamera);
	}*/
}