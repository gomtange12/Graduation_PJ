//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "DDSTextureLoader12.h"
CShader::CShader()
{
	m_d3dSrvCPUDescriptorStartHandle.ptr = NULL;
	m_d3dSrvGPUDescriptorStartHandle.ptr = NULL;
}

CShader::~CShader()
{
	if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBlob = nullptr;

	::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, &errorBlob);

	if (errorBlob)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = NULL;
	d3dInputLayoutDesc.NumElements = 0;

	return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState()
{
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

	return(d3dDepthStencilDesc);
}

D3D12_BLEND_DESC CShader::CreateBlendState()
{
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

	return(d3dBlendDesc);
}

void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_pd3dPipelineState);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::CreateCbvAndSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

void CShader::CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCPUDescriptorHandle;
		d3dCbvCPUDescriptorHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * j);
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorHandle);
	}
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

void CShader::CreateShaderResourceViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_d3dSrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorStartHandle;
	int nTextures = pTexture->GetTextures(); //텍스쳐 생성하면 인자가1이지 이게 여기에 들어옴. 트루로해놓으면 맨처음 srv는 한번 다음거는 4번 그다음거는 5번.그럼 루트파라미터 345.
	int nTextureType = pTexture->GetTextureType(); 
	for (int i = 0; i < nTextures; i++)//1개면 0부터1까지돔. 맨처음 인자값i가 0번째 텍스쳐에 8번파라미터에연결하고.
	//디스크립터힙안에 0번째인덱스를 리소스를 연결해주는거. 그 다음에 ptrdms 64만큼 더해주고 이동시켜줌. 그런식으로 srv저장하는거. 나중에 업데이트 쉐이더베리어블에서 setrootdiscriptortable해주고.  여길 읽어서 드로우인스턴스해라. 
	{
		//디스크립터테이블만들어서 포인터로 넘기면 가능. 이미지텍스쳐같은경우엔 테이블로밖에 못넘기는건데 인스턴싱정보를 srv로 넘겨주는건 32bit짜리로 가능하기때문에. 
		ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
		D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
		pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);
		d3dSrvCPUDescriptorHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

		pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameterStartIndex + i) : nRootParameterStartIndex, d3dSrvGPUDescriptorHandle);
		d3dSrvGPUDescriptorHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
	}
}
void CShader::CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}
void CShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
}

void CShader::ReleaseShaderVariables()
{
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();
}

void CShader::ReleaseUploadBuffers()
{
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_pd3dPipelineState) pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	UpdateShaderVariables(pd3dCommandList);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSPlayer", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSPlayer", "ps_5_1", ppd3dShaderBlob));
}

void CPlayerShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTexturedShader::CTexturedShader()
{
}

CTexturedShader::~CTexturedShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTexturedShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTexturedShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextured", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTexturedShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextured", "ps_5_1", ppd3dShaderBlob));
}

void CTexturedShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CObjectsShader::CObjectsShader()
{
}

CObjectsShader::~CObjectsShader()
{
}

void CObjectsShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
}

void CObjectsShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	for (int j = 0; j < m_nObjects; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
	}
	
}

void CObjectsShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

void CObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{



}

void CObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) delete m_pMaterial;
#endif
}

void CObjectsShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Animate(fTimeElapsed, pCamera);
	}
}

void CObjectsShader::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->ReleaseUploadBuffers();
#endif
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);
#endif

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera, m_nObjects);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 4;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] ={ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTerrain", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTerrain", "ps_5_1", ppd3dShaderBlob));
}

void CTerrainShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CTerrainShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	//XMStoreFloat4x4(&m_pcbMappedGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSkyBoxShader::CSkyBoxShader()
{
}

CSkyBoxShader::~CSkyBoxShader()
{
}

D3D12_DEPTH_STENCIL_DESC CSkyBoxShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

D3D12_SHADER_BYTECODE CSkyBoxShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSSkyBox", "ps_5_1", ppd3dShaderBlob));
}

void CSkyBoxShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

// instancing shader - 여기서부터 미완성주석
//CInstancingShader::CInstancingShader()
//{
//}
//
//CInstancingShader::~CInstancingShader()
//{
//}
//
//D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
//{
//
//	UINT nInputElementDescs = 2;
//	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
//	//정점 정보를 위한 입력 원소이다.
//	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//
//	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
//	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
//	d3dInputLayoutDesc.NumElements = nInputElementDescs;
//	return(d3dInputLayoutDesc);
//
//}
//
//D3D12_SHADER_BYTECODE CInstancingShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
//{
//	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
//		ppd3dShaderBlob));
//}
//
//D3D12_SHADER_BYTECODE CInstancingShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
//{
//	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
//		ppd3dShaderBlob));
//}
//
//void CInstancingShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
//{
//	m_nPipelineStates = 1;
//	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
//	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
//}
//
//void CInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
//{
//	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성
//	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
//		sizeof(VS_VB_INSTANCE) * m_nTree, D3D12_HEAP_TYPE_UPLOAD,
//		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//	//여기. D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,이것도 다르다. 
//	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
//	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedTreeObjects);
//
//	//정점 버퍼에 대한 뷰를 생성한다.
//	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbGameObjects->GetGPUVirtualAddress();
//	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
//	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE) * m_nObjects;
//}
//
//void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
//{
//
//	//pd3dCommandList->SetGraphicsRootShaderResourceView(6,m_pd3dcbGameObjects->GetGPUVirtualAddress());
//	////for (int j = 0; j < m_n; j++)
//	//{
//	//	/*m_pcbMappedGameObjects[j].m_xmColor = (j % 2) ? XMFLOAT4(0.5f, 0.0f, 0.0f, 0.0f) :
//	//		XMFLOAT4(0.0f, 0.0f, 0.5f, 0.0f);*/
//	//	XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
//	//		XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
//	//}
//}
//
//void CInstancingShader::ReleaseShaderVariables()
//{
//	if (m_pd3dcbGameObjects) m_pd3dcbGameObjects->Unmap(0, NULL);
//	if (m_pd3dcbGameObjects) m_pd3dcbGameObjects->Release();
//}
//
//void CInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void* HeightMap)
//{
//	
//}
//
//void CInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera, UINT nObjects)
//{
//
//}
//
//void CInstancingShader::AnimateObjects(float fTimeElapsed)
//{
//
//
//
//}

////풀
//CGrassInstancingShader::CGrassInstancingShader()
//{
//}
//
//CGrassInstancingShader::~CGrassInstancingShader()
//{
//}
//
//D3D12_INPUT_LAYOUT_DESC CGrassInstancingShader::CreateInputLayout()
//{
//
//	UINT nInputElementDescs = 2;
//	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
//	//정점 정보를 위한 입력 원소이다.
//	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//
//	
//	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
//	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
//	d3dInputLayoutDesc.NumElements = nInputElementDescs;
//	return(d3dInputLayoutDesc);
//
//}
//
//D3D12_SHADER_BYTECODE CGrassInstancingShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
//{
//	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
//		ppd3dShaderBlob));
//}
//
//D3D12_SHADER_BYTECODE CGrassInstancingShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
//{
//	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
//		ppd3dShaderBlob));
//}
//
//void CGrassInstancingShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
//{
//	m_nPipelineStates = 1;
//	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
//	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
//}
//
//void CGrassInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
//{
//	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성
//	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
//		sizeof(VS_VB_INSTANCE) * m_nGrass, D3D12_HEAP_TYPE_UPLOAD,
//		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//	//여기. D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,이것도 다르다. 
//	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
//	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
//
//	//정점 버퍼에 대한 뷰를 생성한다.
//	m_d3dInstancingBufferView.BufferLocation = m_pd3dcbGameObjects->GetGPUVirtualAddress();
//	m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
//	m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE) * m_nObjects;
//}
//void CGrassInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
//{
//
//	for (int j = 0; j < m_nObjects; j++)
//	{
//		/*m_pcbMappedGameObjects[j].m_xmColor = (j % 2) ? XMFLOAT4(0.5f, 0.0f, 0.0f, 0.0f) :
//			XMFLOAT4(0.0f, 0.0f, 0.5f, 0.0f);*/
//		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
//			XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
//	}
//	pd3dCommandList->SetGraphicsRootShaderResourceView(2, m_pd3dcbGameObjects->GetGPUVirtualAddress());
//}
//void CGrassInstancingShader::ReleaseShaderVariables()
//{
//	if (m_pd3dcbGameObjects) m_pd3dcbGameObjects->Unmap(0, NULL);
//	if (m_pd3dcbGameObjects) m_pd3dcbGameObjects->Release();
//}
//
//void CGrassInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void* pHeightMap)
//{
//	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
//	/*CTexturedRectMesh *pCubeMeshTextured = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10,10,0,0,-10);
//	m_ppObjects = new CGameObject*[m_nObjects];
//
//	*/
//	CHeightMapTerrain *pTerrain = (CHeightMapTerrain*)pHeightMap;
//	int xObjects = 10, yObjects = 10, zObjects = 10, i = 0;
//
//	float fxPitch = 12.0f * 3.5f;
//	float fyPitch = 12.0f * 3.5f;
//	float fzPitch = 12.0f * 3.5f;
//
//	float fTerrainWidth = pTerrain->GetWidth();
//	float fTerrainLength = pTerrain->GetLength();
//
//	m_nGrass = (xObjects * yObjects * zObjects);
//
//
//	CCubeMeshTextured *pCubeMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 12.0f, 12.0f, 12.0f);
//	CTexture *pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
//	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass02.dds", 0);
//
//	m_ppGrass = new CGameObject*[m_nGrass];
//	
//	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
//	CRotatingObject *pRotatingObject = NULL;
//	for (int i = 0, x = 0; x < xObjects; x++)
//	{
//		for (int z = 0; z < zObjects; z++)
//		{
//			for (int y = 0; y < yObjects; y++)
//			{
//				pRotatingObject = new CRotatingObject(1);
//				pRotatingObject->SetMesh(0, pCubeMesh);
//	#ifndef _WITH_BATCH_MATERIAL
//					pRotatingObject->SetMaterial(pCubeMaterial);
//	#endif
//				float xPosition = x * fxPitch;
//				float zPosition = z * fzPitch;
//				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
//				pRotatingObject->SetPosition(xPosition, fHeight + (y * 3.0f * fyPitch) + 6.0f, zPosition);
//				if (y == 0)
//				{
//					xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
//					xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal);
//					if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
//					float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal));
//					pRotatingObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
//				}
//				pRotatingObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
//				pRotatingObject->SetRotationSpeed(36.0f * (i % 10) + 36.0f);
//				pRotatingObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
//				m_ppGrass[i++] = pRotatingObject;
//			}
//		}
//	}
//
//	UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);
//
//	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nGrass, 1);
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);
//	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
//	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 3, false);
//}
//
//void CGrassInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
//{
//	CObjectsShader::Render(pd3dCommandList, pCamera);
//	UpdateShaderVariables(pd3dCommandList);
//	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
//	m_ppGrass[0]->Render(pd3dCommandList, pCamera, m_nGrass);
//}
//
//void CGrassInstancingShader::AnimateObjects(float fTimeElapsed)
//{
//	for (int i = 0; i < m_nGrass; ++i)
//	{
//		m_ppGrass[i]->Animate(fTimeElapsed);
//	}
//}

CBillboardObjectsShader::CBillboardObjectsShader()
{
}

CBillboardObjectsShader::~CBillboardObjectsShader()
{
}

D3D12_RASTERIZER_DESC CBillboardObjectsShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}
void CBillboardObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	//float fTerrainWidth = pTerrain->GetWidth();
	//float fTerrainLength = pTerrain->GetLength();
	//int xObjects = 35;
	//int zObjects = 50;
	//m_nObjects = (xObjects * zObjects);
	//CTexture *ppGrassTextures[2];
	//ppGrassTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppGrassTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass01.dds", 0);
	//ppGrassTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppGrassTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass02.dds", 0);
	//CTexture *ppFlowerTextures[2];
	//ppFlowerTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppFlowerTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower01.dds", 0);
	//ppFlowerTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppFlowerTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower02.dds", 0);
	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	//CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 4);
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[0], 3, true);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[1], 3, true);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[1], 3, true);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[0], 3, true);
	//CMaterial *ppGrassMaterials[2];
	//ppGrassMaterials[0] = new CMaterial();
	//ppGrassMaterials[0]->SetTexture(ppGrassTextures[0]);
	//ppGrassMaterials[1] = new CMaterial();
	//ppGrassMaterials[1]->SetTexture(ppGrassTextures[1]);
	//CMaterial *ppFlowerMaterials[2];
	//ppFlowerMaterials[0] = new CMaterial();
	//ppFlowerMaterials[0]->SetTexture(ppFlowerTextures[0]);
	//ppFlowerMaterials[1] = new CMaterial();
	//ppFlowerMaterials[1]->SetTexture(ppFlowerTextures[1]);
	//CTexturedRectMesh *pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//CTexturedRectMesh *pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//m_ppObjects = new CGameObject*[m_nObjects];
	//CBillboardObject *pBillboardObject = NULL;
	//float fxPitch = 0.25f;
	//float fzPitch = 0.25f;
	//CMaterial *pMaterial = NULL;
	//CMesh *pMesh = NULL;
	//float xPosition = 0.0f;
	//for (int i = 0, x = 0; x < xObjects; x++)
	//{
	//	xPosition += x * fxPitch;
	//	float zPosition = 130.0f;
	//	float xPosition = x * fxPitch;
	//	//float zPosition = z * fzPitch;
	//	float fHeight = pTerrain->GetHeight(xPosition, zPosition);
	//	//pRotatingObject->SetPosition(200, fHeight + (y * 3.0f * fyPitch) + 6.0f, zPosition);
	//	for (int z = 0; z < zObjects; z++)
	//	{
	//		pBillboardObject = new CBillboardObject();
	//		pBillboardObject->SetMesh(0, (z % 2) ? pGrassMesh : pFlowerMesh);
	//		if (x % 2) pMaterial = (z % 2) ? ppGrassMaterials[0] : ppFlowerMaterials[1];
	//		else pMaterial = (z % 2) ? ppGrassMaterials[1] : ppFlowerMaterials[0];
	//		pBillboardObject->SetMaterial(pMaterial);
	//		zPosition += z * fzPitch;
	//		float fHeight = pTerrain->GetHeight(xPosition, zPosition);
	//		pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 8.0f : 16.0f) * 0.5f), zPosition);
	//		pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
	//		m_ppObjects[i++] = pBillboardObject;
	//	}
	//}
}

void CBillboardObjectsShader::ReleaseUploadBuffers()
{
	CObjectsShader::ReleaseUploadBuffers();
}

void CBillboardObjectsShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();
}

void CBillboardObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->SetLookAt(xmf3CameraPosition, XMFLOAT3(0.0f, 1.0f, 0.0f));
	}

	CObjectsShader::Render(pd3dCommandList, pCamera);
}

CInstancingShader::CInstancingShader()
{
}

CInstancingShader::~CInstancingShader()
{
}
D3D12_DEPTH_STENCIL_DESC CInstancingShader::CreateDepthStencilState()
{
	//여기
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
	//여기
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
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
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
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);

	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbGrassObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_VB_INSTANCE) * m_nGrass, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbGrassObjects->Map(0, NULL, (void **)&m_pcbMappedGrassObjects);

	////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbTreeObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nTree;
}

void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	

	//pd3dCommandList->SetGraphicsRootDescriptorTable(m_pTexture->m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);
	
	//테이블로 연결해서 넘겨야함. 
	for (int j = 0; j < m_nGrass; j++)
	{
		XMStoreFloat4x4(&m_pcbMappedGrassObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppGrass[j]->m_xmf4x4World)));
	}
	pd3dCommandList->SetGraphicsRootShaderResourceView(7, //여기가 2아니라 3
		m_pd3dcbGrassObjects->GetGPUVirtualAddress());
}

void CInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGrassObjects) m_pd3dcbGrassObjects->Unmap(0, NULL);
	if (m_pd3dcbGrassObjects) m_pd3dcbGrassObjects->Release();
}

void CInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void *pHeightMap)
{
//	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;
//
//	float fxPitch = 12.0f * 3.5f;
//	float fyPitch = 12.0f * 3.5f;
//	float fzPitch = 12.0f * 3.5f;
//
//	float fTerrainWidth = pTerrain->GetWidth();
//	float fTerrainLength = pTerrain->GetLength();
//
//	int xObjects = int(fTerrainWidth / fxPitch);
//	int yObjects = 2;
//	int zObjects = int(fTerrainLength / fzPitch);
//	m_nTree = (xObjects * yObjects * zObjects);
//	//m_nObjects = 123;
//	//m_nTree = 100;
//	CTexture *pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
//	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass02.dds", 0);
//
//	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
//
//	//여기를 보자
//	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList,0, 1); //여기가 0이 되어야.....ㅇㅕ기다 여기
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);
//	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
//	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 3, false); //루트파라미터 인덱스 잘 확인하고 
//
//#ifdef _WITH_BATCH_MATERIAL
//	m_pMaterial = new CMaterial();
//	m_pMaterial->SetTexture(pTexture);
//#else
//	CMaterial *pCubeMaterial = new CMaterial();
//	pCubeMaterial->SetTexture(pTexture);
//#endif
//
//	CTexturedRectMesh *pTexturedRect = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 12.0f, 12.0f, 12.0f);
//
//	m_ppTree = new CGameObject*[m_nTree];
//
//
//	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
//	CBillboardObject *pBillboardObject = NULL;
//	for (int i = 0, x = 0; x < xObjects; x++)
//	{
//		float xPosition = 0.0f; // 깔리는 위치 지정
//		float zPosition = 0.0f; // 깔리는 위치 지정
//
//		for (int z = 0; z < zObjects; z++)
//		{
//
//			for (int y = 0; y < yObjects; y++)
//			{
//				pBillboardObject = new CBillboardObject(1);
//				pBillboardObject->SetMesh(0, pTexturedRect);
//#ifndef _WITH_BATCH_MATERIAL
//				pRotatingObject->SetMaterial(pCubeMaterial);
//#endif
//				 xPosition = x * fxPitch;
//				 zPosition = z * fzPitch;
//				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
//				pBillboardObject->SetPosition(xPosition, fHeight + (y * 3.0f * fyPitch) + 6.0f, zPosition);
//				if (y == 0)
//				{
//					xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
//					xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal);
//					if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
//					float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal));
//					pBillboardObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
//				}
//
//				pBillboardObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
//				pBillboardObject->SetRotationSpeed(36.0f * (i % 10) + 36.0f);
//				pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
//				m_ppTree[i++] = pBillboardObject;
//			}
//		}
//	}
//
//	// 인스턴신용 메쉬 하나만 처리
//	m_ppTree[0]->SetMesh(0, pTexturedRect);
//	//for (int i = 0; i < m_nTree; ++i)
//	//	m_ppTree[i]->SetMesh(0, pTexturedRect);
//
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);//이걸 빼먹어버림

	//된거
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	//오브젝트의 개수에 따라서 맵전체에 깔리는게 바뀜
	int xObjects = 160;
	int zObjects = 160;
	m_nGrass = (xObjects * zObjects);

	CTexture *ppGrassTextures;
	ppGrassTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppGrassTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass01.dds", 0);
	/*ppGrassTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppGrassTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass02.dds", 0);
	CTexture *ppFlowerTextures[2];
	ppFlowerTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower01.dds", 0);
	ppFlowerTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower02.dds", 0);*/

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // 텍스쳐 srv
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures, 3, true);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[1], 3, true);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[0], 3, true);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[1], 3, true); // 맨 마지막 텍스쳐 하나만 그려지는게 당연하네... 결국 얘만 되는중이었음


	CMaterial *ppGrassMaterials;
	ppGrassMaterials = new CMaterial();
	ppGrassMaterials->SetTexture(ppGrassTextures);
	/*ppGrassMaterials[1] = new CMaterial();
	ppGrassMaterials[1]->SetTexture(ppGrassTextures[1]);*/





	/*CMaterial *ppFlowerMaterials[2];

	ppFlowerMaterials[0] = new CMaterial();
	ppFlowerMaterials[0]->SetTexture(ppFlowerTextures[0]);
	ppFlowerMaterials[1] = new CMaterial();
	ppFlowerMaterials[1]->SetTexture(ppFlowerTextures[1]);*/


	CTexturedRectMesh *pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 5.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//CTexturedRectMesh *pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 5.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f); // 빌보드의 크기 설정

	m_ppGrass = new CGameObject*[m_nGrass];

	CGrassObject *pBillboardObject = NULL;

	//깔리는 밀도 조절됨
	float fxPitch = 0.15f;
	float fzPitch = 0.15f;

	CMaterial *pMaterial = NULL;
	CMesh *pMesh = NULL;

	float xPosition = 0.0f; // 깔리는 위치 지정
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		xPosition += x * fxPitch;
		float zPosition = 0.0f;
		for (int z = 0; z < zObjects; z++)
		{
			pBillboardObject = new CGrassObject();

			//pBillboardObject->SetMesh(0, pFlowerMesh);

			//if (x % 2) pMaterial = (z % 2) ? ppGrassMaterials[0] : ppFlowerMaterials[1];
			//else pMaterial = (z % 2) ? ppGrassMaterials[1] : ppFlowerMaterials[0]; // z가 0이면 플라워 1dds, z가 1이면 그래스 2dds를 머터리얼 셋한다.
			pBillboardObject->SetMaterial(ppGrassMaterials);

			zPosition += z * fzPitch;
			float fHeight = pTerrain->GetHeight(xPosition, zPosition);
			pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 4.0f : 8.0f) * 0.5f), zPosition); // 셋포지션의 헤이트값 조절해서 바닥에서 자라는지 설정. 풀 월드포지션 설정해주는 부분임
			pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			m_ppGrass[i++] = pBillboardObject;

		}
	}
	m_ppGrass[0]->SetMesh(0, pGrassMesh);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);//이걸 빼먹어버림

}

void CInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	//OnPrepareRender(pd3dCommandList);

	CObjectsShader::Render(pd3dCommandList, pCamera);
	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);
	//CTexturedShader::Render(pd3dCommandList, pCamera);

	
	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppGrass[0]->Render(pd3dCommandList, pCamera, m_nGrass); //여기
}

void CInstancingShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	for (int i = 0; i < m_nGrass; ++i)
	{
		m_ppGrass[i]->Animate(fTimeElapsed,pCamera);
	}
}


////////Tree1//////////////////////////
CTreeInstancingShader::CTreeInstancingShader()
{
}

CTreeInstancingShader::~CTreeInstancingShader()
{
}
D3D12_DEPTH_STENCIL_DESC CTreeInstancingShader::CreateDepthStencilState()
{
	//여기
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

D3D12_BLEND_DESC CTreeInstancingShader::CreateBlendState()
{
	//여기
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
D3D12_INPUT_LAYOUT_DESC CTreeInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTreeInstancingShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
		ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTreeInstancingShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
		ppd3dShaderBlob));
}

void CTreeInstancingShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CTreeInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);

	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbTreeObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_VB_INSTANCE) * m_nTree, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbTreeObjects->Map(0, NULL, (void **)&m_pcbMappedTreeObjects);

	////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbTreeObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nTree;
}

void CTreeInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{


	//pd3dCommandList->SetGraphicsRootDescriptorTable(m_pTexture->m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);

	//테이블로 연결해서 넘겨야함. 
	for (int j = 0; j < m_nTree; j++)
	{
		XMStoreFloat4x4(&m_pcbMappedTreeObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppTree[j]->m_xmf4x4World)));
	}
	pd3dCommandList->SetGraphicsRootShaderResourceView(7, //여기가 2아니라 3
		m_pd3dcbTreeObjects->GetGPUVirtualAddress());
}

void CTreeInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbTreeObjects) m_pd3dcbTreeObjects->Unmap(0, NULL);
	if (m_pd3dcbTreeObjects) m_pd3dcbTreeObjects->Release();
}

void CTreeInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void *pHeightMap)
{

	//된거
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	//오브젝트의 개수에 따라서 맵전체에 깔리는게 바뀜
	int xObjects = 100;
	int zObjects = 50;
	m_nTree = (xObjects * zObjects);

	CTexture *ppTreeTextures;
	ppTreeTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/TREE.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // 텍스쳐 srv
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTreeTextures, 3, true);
	

	CMaterial *ppTreeMaterials;
	ppTreeMaterials = new CMaterial();
	ppTreeMaterials->SetTexture(ppTreeTextures);
	
	// 빌보드의 크기 설정
	CTexturedRectMesh *pTreeMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 15.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_ppTree = new CGameObject*[m_nTree];

	CTreeObject *pBillboardObject = NULL;

	//깔리는 밀도 조절됨
	float fxPitch = 0.15f;
	float fzPitch = 0.15f;

	CMaterial *pMaterial = NULL;
	CMesh *pMesh = NULL;

	float xPosition = 400.0f; // 깔리는 위치 지정
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		xPosition += x * fxPitch;
		float zPosition = 500.0f;
		for (int z = 0; z < zObjects; z++)
		{
			pBillboardObject = new CTreeObject();

			pBillboardObject->SetMaterial(ppTreeMaterials);

			zPosition += z * fzPitch;
			float fHeight = pTerrain->GetHeight(xPosition, zPosition);
			pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 45.0f : 40.0f) * 0.5f), zPosition); // 셋포지션의 헤이트값 조절해서 바닥에서 자라는지 설정. 풀 월드포지션 설정해주는 부분임
			pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			m_ppTree[i++] = pBillboardObject;

		}
	}
	m_ppTree[0]->SetMesh(0, pTreeMesh);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);//이걸 빼먹어버림

}

void CTreeInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	//OnPrepareRender(pd3dCommandList);

	CObjectsShader::Render(pd3dCommandList, pCamera);
	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);
	//CTexturedShader::Render(pd3dCommandList, pCamera);


	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppTree[0]->Render(pd3dCommandList, pCamera, m_nTree); //여기
}

void CTreeInstancingShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	for (int i = 0; i < m_nTree; ++i)
	{
		m_ppTree[i]->Animate(fTimeElapsed, pCamera);
	}
}

////////////벛꽃나무
////////Tree2//////////////////////////
CFlowerTreeInstancingShader::CFlowerTreeInstancingShader()
{
}

CFlowerTreeInstancingShader::~CFlowerTreeInstancingShader()
{
}
D3D12_DEPTH_STENCIL_DESC CFlowerTreeInstancingShader::CreateDepthStencilState()
{
	//여기
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

D3D12_BLEND_DESC CFlowerTreeInstancingShader::CreateBlendState()
{
	//여기
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
D3D12_INPUT_LAYOUT_DESC CFlowerTreeInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CFlowerTreeInstancingShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
		ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CFlowerTreeInstancingShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
		ppd3dShaderBlob));
}

void CFlowerTreeInstancingShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CFlowerTreeInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);

	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbFlowerTreeObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_VB_INSTANCE) * m_nFlowerTree, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbFlowerTreeObjects->Map(0, NULL, (void **)&m_pcbMappedFlowerTreeObjects);

	////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbTreeObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nTree;
}

void CFlowerTreeInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{


	//pd3dCommandList->SetGraphicsRootDescriptorTable(m_pTexture->m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);

	//테이블로 연결해서 넘겨야함. 
	for (int j = 0; j < m_nFlowerTree; j++)
	{
		XMStoreFloat4x4(&m_pcbMappedFlowerTreeObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppFlowerTree[j]->m_xmf4x4World)));
	}
	pd3dCommandList->SetGraphicsRootShaderResourceView(7, //여기가 2아니라 3
		m_pd3dcbFlowerTreeObjects->GetGPUVirtualAddress());
}

void CFlowerTreeInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbFlowerTreeObjects) m_pd3dcbFlowerTreeObjects->Unmap(0, NULL);
	if (m_pd3dcbFlowerTreeObjects) m_pd3dcbFlowerTreeObjects->Release();
}

void CFlowerTreeInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void *pHeightMap)
{

	//된거
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	//오브젝트의 개수에 따라서 맵전체에 깔리는게 바뀜
	int xObjects = 110;
	int zObjects = 70;
	m_nFlowerTree = (xObjects * zObjects);

	CTexture *ppFlowerTreeTextures;
	ppFlowerTreeTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTreeTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/blossom1.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // 텍스쳐 srv
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTreeTextures, 3, true);


	CMaterial *ppFlowerTreeMaterials;
	ppFlowerTreeMaterials = new CMaterial();
	ppFlowerTreeMaterials->SetTexture(ppFlowerTreeTextures);

	// 빌보드의 크기 설정
	CTexturedRectMesh *pTreeMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 15.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_ppFlowerTree = new CGameObject*[m_nFlowerTree];

	CTreeObject *pBillboardObject = NULL;

	//깔리는 밀도 조절됨
	float fxPitch = 0.35f;
	float fzPitch = 0.45f;

	CMaterial *pMaterial = NULL;
	CMesh *pMesh = NULL;

	float xPosition = 0.0f; // 깔리는 위치 지정
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		xPosition += x * fxPitch;
		float zPosition = 900.0f;
		for (int z = 0; z < zObjects; z++)
		{
			pBillboardObject = new CTreeObject();

			pBillboardObject->SetMaterial(ppFlowerTreeMaterials);

			zPosition += z * fzPitch;
			float fHeight = pTerrain->GetHeight(xPosition, zPosition);
			pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 25.0f : 30.0f) * 0.5f), zPosition); // 셋포지션의 헤이트값 조절해서 바닥에서 자라는지 설정. 풀 월드포지션 설정해주는 부분임
			pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			m_ppFlowerTree[i++] = pBillboardObject;

		}
	}
	m_ppFlowerTree[0]->SetMesh(0, pTreeMesh);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);//이걸 빼먹어버림

}

void CFlowerTreeInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	//OnPrepareRender(pd3dCommandList);

	CObjectsShader::Render(pd3dCommandList, pCamera);
	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);
	//CTexturedShader::Render(pd3dCommandList, pCamera);


	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppFlowerTree[0]->Render(pd3dCommandList, pCamera, m_nFlowerTree); //여기
}

void CFlowerTreeInstancingShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	for (int i = 0; i < m_nFlowerTree; ++i)
	{
		m_ppFlowerTree[i]->Animate(fTimeElapsed, pCamera);
	}
}

////////////벛꽃잎
////////잔디2//////////////////////////
CFlowerTreeleafInstancingShader::CFlowerTreeleafInstancingShader()
{
}

CFlowerTreeleafInstancingShader::~CFlowerTreeleafInstancingShader()
{
}
D3D12_DEPTH_STENCIL_DESC CFlowerTreeleafInstancingShader::CreateDepthStencilState()
{
	//여기
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

D3D12_BLEND_DESC CFlowerTreeleafInstancingShader::CreateBlendState()
{
	//여기
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
D3D12_INPUT_LAYOUT_DESC CFlowerTreeleafInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CFlowerTreeleafInstancingShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1",
		ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CFlowerTreeleafInstancingShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1",
		ppd3dShaderBlob));
}

void CFlowerTreeleafInstancingShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CFlowerTreeleafInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);

	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbFlowerTreeleafObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_VB_INSTANCE) * m_nFlowerTreeleaf, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbFlowerTreeleafObjects->Map(0, NULL, (void **)&m_pcbMappedFlowerTreeleafObjects);

	////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbTreeObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nTree;
}

void CFlowerTreeleafInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{


	//pd3dCommandList->SetGraphicsRootDescriptorTable(m_pTexture->m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);

	//테이블로 연결해서 넘겨야함. 
	for (int j = 0; j < m_nFlowerTreeleaf; j++)
	{
		XMStoreFloat4x4(&m_pcbMappedFlowerTreeleafObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&m_ppFlowerTreeleaf[j]->m_xmf4x4World)));
	}
	pd3dCommandList->SetGraphicsRootShaderResourceView(7, //여기가 2아니라 3
		m_pd3dcbFlowerTreeleafObjects->GetGPUVirtualAddress());
}

void CFlowerTreeleafInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbFlowerTreeleafObjects) m_pd3dcbFlowerTreeleafObjects->Unmap(0, NULL);
	if (m_pd3dcbFlowerTreeleafObjects) m_pd3dcbFlowerTreeleafObjects->Release();
}

void CFlowerTreeleafInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void *pHeightMap)
{

	//된거
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	//오브젝트의 개수에 따라서 맵전체에 깔리는게 바뀜
	int xObjects = 100;
	int zObjects = 60;
	m_nFlowerTreeleaf = (xObjects * zObjects);

	CTexture *ppFlowerTreeTextures;
	ppFlowerTreeTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTreeTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/blossomleaf2.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // 텍스쳐 srv
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTreeTextures, 3, true);


	CMaterial *ppFlowerTreeleafMaterials;
	ppFlowerTreeleafMaterials = new CMaterial();
	ppFlowerTreeleafMaterials->SetTexture(ppFlowerTreeTextures);

	// 빌보드의 크기 설정
	CTexturedRectMesh *pTreeMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 15.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_ppFlowerTreeleaf = new CGameObject*[m_nFlowerTreeleaf];

	CTreeObject *pBillboardObject = NULL;

	//깔리는 밀도 조절됨
	float fxPitch = 0.40f;
	float fzPitch = 0.40f;

	CMaterial *pMaterial = NULL;
	CMesh *pMesh = NULL;

	float xPosition = 0.0f; // 깔리는 위치 지정
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		xPosition += x * fxPitch;
		float zPosition = 900.0f;
		for (int z = 0; z < zObjects; z++)
		{
			pBillboardObject = new CTreeObject();

			pBillboardObject->SetMaterial(ppFlowerTreeleafMaterials);

			zPosition += z * fzPitch;
			float fHeight = pTerrain->GetHeight(xPosition, zPosition);
			pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 4.0f : 5.0f) * 0.5f), zPosition); // 셋포지션의 헤이트값 조절해서 바닥에서 자라는지 설정. 풀 월드포지션 설정해주는 부분임
			pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			m_ppFlowerTreeleaf[i++] = pBillboardObject;

		}
	}
	m_ppFlowerTreeleaf[0]->SetMesh(0, pTreeMesh);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);//이걸 빼먹어버림

}

void CFlowerTreeleafInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	//OnPrepareRender(pd3dCommandList);

	CObjectsShader::Render(pd3dCommandList, pCamera);
	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);
	//CTexturedShader::Render(pd3dCommandList, pCamera);


	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_ppFlowerTreeleaf[0]->Render(pd3dCommandList, pCamera, m_nFlowerTreeleaf); //여기
}

void CFlowerTreeleafInstancingShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	for (int i = 0; i < m_nFlowerTreeleaf; ++i)
	{
		m_ppFlowerTreeleaf[i]->Animate(fTimeElapsed, pCamera);
	}
}