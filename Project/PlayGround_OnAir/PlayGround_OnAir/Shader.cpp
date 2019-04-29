//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"

#include "Player.h"
#include "CPlayerManager.h"
CShader::CShader()
{
}

CShader::~CShader()
{
	ReleaseShaderVariables();

	if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader()
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader()
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

	ID3DBlob *pd3dErrorBlob = NULL;
	HRESULT hResult = ::D3DCompileFromFile(pszFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, &pd3dErrorBlob);
	char *pErrorString = NULL;
	if (pd3dErrorBlob) pErrorString = (char *)pd3dErrorBlob->GetBufferPointer();

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

#define _WITH_WFOPEN
//#define _WITH_STD_STREAM

#ifdef _WITH_STD_STREAM
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#endif

D3D12_SHADER_BYTECODE CShader::ReadCompiledShaderFromFile(WCHAR *pszFileName, ID3DBlob **ppd3dShaderBlob)
{
	UINT nReadBytes = 0;
#ifdef _WITH_WFOPEN
	FILE *pFile = NULL;
	::_wfopen_s(&pFile, pszFileName, L"rb");
	::fseek(pFile, 0, SEEK_END);
	int nFileSize = ::ftell(pFile);
	BYTE *pByteCode = new BYTE[nFileSize];
	::rewind(pFile);
	nReadBytes = (UINT)::fread(pByteCode, sizeof(BYTE), nFileSize, pFile);
	::fclose(pFile);
#endif
#ifdef _WITH_STD_STREAM
	std::ifstream ifsFile;
	ifsFile.open(pszFileName, std::ios::in | std::ios::ate | std::ios::binary);
	nReadBytes = (int)ifsFile.tellg();
	BYTE *pByteCode = new BYTE[*pnReadBytes];
	ifsFile.seekg(0);
	ifsFile.read((char *)pByteCode, nReadBytes);
	ifsFile.close();
#endif

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	if (ppd3dShaderBlob)
	{
		*ppd3dShaderBlob = NULL;
		HRESULT hResult = D3DCreateBlob(nReadBytes, ppd3dShaderBlob);
		memcpy((*ppd3dShaderBlob)->GetBufferPointer(), pByteCode, nReadBytes);
		d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
		d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
	}
	else
	{
		d3dShaderByteCode.BytecodeLength = nReadBytes;
		d3dShaderByteCode.pShaderBytecode = pByteCode;
	}

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
	//	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
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

void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	m_d3dPipelineStateDesc.VS = CreateVertexShader();
	m_d3dPipelineStateDesc.PS = CreatePixelShader();
	m_d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	m_d3dPipelineStateDesc.BlendState = CreateBlendState();
	m_d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	m_d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
	m_d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_d3dPipelineStateDesc.NumRenderTargets = 1;
	m_d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_d3dPipelineStateDesc.SampleDesc.Count = 1;
	m_d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_pd3dPipelineState);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, int nPipelineState)
{
	if (m_pd3dPipelineState) pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dCommandList);
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

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTerrain", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTerrain", "ps_5_1", &m_pd3dPixelShaderBlob));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSkyBoxShader::CSkyBoxShader()
{
}

CSkyBoxShader::~CSkyBoxShader()
{
}

D3D12_INPUT_LAYOUT_DESC CSkyBoxShader::CreateInputLayout()
{
	UINT nInputElementDescs = 1;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
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

D3D12_SHADER_BYTECODE CSkyBoxShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSSkyBox", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE CSkyBoxShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSSkyBox", "ps_5_1", &m_pd3dPixelShaderBlob));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CStandardShader::CStandardShader()
{
}

CStandardShader::~CStandardShader()
{
}

D3D12_INPUT_LAYOUT_DESC CStandardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CStandardShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSStandard", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE CStandardShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSStandard", "ps_5_1", &m_pd3dPixelShaderBlob));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSkinnedAnimationShader::CSkinnedAnimationShader()
{
}

CSkinnedAnimationShader::~CSkinnedAnimationShader()
{
}

D3D12_INPUT_LAYOUT_DESC CSkinnedAnimationShader::CreateInputLayout()
{
	UINT nInputElementDescs = 7;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[5] = { "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 5, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[6] = { "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 6, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CSkinnedAnimationShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSSkinnedAnimationStandard", "vs_5_1", &m_pd3dVertexShaderBlob));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CStandardObjectsShader::CStandardObjectsShader()
{
}

CStandardObjectsShader::~CStandardObjectsShader()
{
}

void CStandardObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
}

void CStandardObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void CStandardObjectsShader::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
}

void CStandardObjectsShader::ReleaseUploadBuffers()
{
	for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
}

void CStandardObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	CStandardShader::Render(pd3dCommandList, pCamera);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->Animate(m_fElapsedTime);
			m_ppObjects[j]->UpdateTransform(NULL);
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}

	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CHellicopterObjectsShader::CHellicopterObjectsShader()
{
}

CHellicopterObjectsShader::~CHellicopterObjectsShader()
{
}

float Random(float fMin, float fMax)
{
	float fRandomValue = (float)rand();
	if (fRandomValue < fMin) fRandomValue = fMin;
	if (fRandomValue > fMax) fRandomValue = fMax;
	return(fRandomValue);
}

float Random()
{
	return(rand() / float(RAND_MAX));
}

XMFLOAT3 RandomPositionInSphere(XMFLOAT3 xmf3Center, float fRadius, int nColumn, int nColumnSpace)
{
    float fAngle = Random() * 360.0f * (2.0f * 3.14159f / 360.0f);

	XMFLOAT3 xmf3Position;
    xmf3Position.x = xmf3Center.x + fRadius * sin(fAngle);
    xmf3Position.y = xmf3Center.y - (nColumn * float(nColumnSpace) / 2.0f) + (nColumn * nColumnSpace) + Random();
    xmf3Position.z = xmf3Center.z + fRadius * cos(fAngle);

	return(xmf3Position);
}

void CHellicopterObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	/*m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	CLoadedModelInfo *pMapModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Map/ALL_PIECES.bin", NULL, false);
	pMapModel->m_pModelRootObject->AddRef();
	int nObjects = 0;
	m_ppObjects[nObjects] = new MapObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_ppObjects[nObjects]->SetPosition(380,100,510);

	m_ppObjects[nObjects]->SetScale(100, 100, 100);*/
	//CLoadedModelInfo *pGunshipModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Gunship.bin", this, false);

	/*int nColumnSpace = 5, nColumnSize = 30;           
    int nFirstPassColumnSize = (m_nObjects % nColumnSize) > 0 ? (nColumnSize - 1) : nColumnSize;

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	int nObjects = 0;
    for (int h = 0; h < nFirstPassColumnSize; h++)
    {
        for (int i = 0; i < floor(float(m_nObjects) / float(nColumnSize)); i++)
        {
			if (nObjects % 2)
			{
				m_ppObjects[nObjects] = new CSuperCobraObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
				m_ppObjects[nObjects]->SetChild(pSuperCobraModel->m_pModelRootObject);
				pSuperCobraModel->m_pModelRootObject->AddRef();
			}
			else
			{
				m_ppObjects[nObjects] = new CGunshipObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
				m_ppObjects[nObjects]->SetChild(pGunshipModel->m_pModelRootObject);
				pGunshipModel->m_pModelRootObject->AddRef();
			}
			float fHeight = pTerrain->GetHeight(390.0f, 670.0f);
			XMFLOAT3 xmf3Position = RandomPositionInSphere(XMFLOAT3(390.0f, fHeight + 35.0f, 670.0f), Random(20.0f, 100.0f), h - int(floor(nColumnSize / 2.0f)), nColumnSpace);
			xmf3Position.y = pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) + Random(0.0f, 25.0f);
			m_ppObjects[nObjects]->SetPosition(xmf3Position);
			m_ppObjects[nObjects]->Rotate(0.0f, 90.0f, 0.0f);
			m_ppObjects[nObjects++]->OnPrepareAnimate();
		}
    }

    if (nFirstPassColumnSize != nColumnSize)
    {
        for (int i = 0; i < m_nObjects - int(floor(float(m_nObjects) / float(nColumnSize)) * nFirstPassColumnSize); i++)
        {
			if (nObjects % 2)
			{
				m_ppObjects[nObjects] = new CSuperCobraObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
				m_ppObjects[nObjects]->SetChild(pSuperCobraModel->m_pModelRootObject);
				pSuperCobraModel->m_pModelRootObject->AddRef();
			}
			else
			{
				m_ppObjects[nObjects] = new CGunshipObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
				m_ppObjects[nObjects]->SetChild(pGunshipModel->m_pModelRootObject);
				pGunshipModel->m_pModelRootObject->AddRef();
			}
			m_ppObjects[nObjects]->SetPosition(RandomPositionInSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), Random(20.0f, 100.0f), nColumnSize - int(floor(nColumnSize / 2.0f)), nColumnSpace));
			m_ppObjects[nObjects]->Rotate(0.0f, 90.0f, 0.0f);
			m_ppObjects[nObjects++]->OnPrepareAnimate();
        }
    }*/

	//CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//if (pMapModel) delete pMapModel;
	//if (pGunshipModel) delete pGunshipModel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSkinnedAnimationObjectsShader::CSkinnedAnimationObjectsShader()
{
}

CSkinnedAnimationObjectsShader::~CSkinnedAnimationObjectsShader()
{
}

void CSkinnedAnimationObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{

}

void CSkinnedAnimationObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void CSkinnedAnimationObjectsShader::AnimateObjects(float fTimeElapsed)
{
	//if (!m_ppObjects[0] )
	//{

	//if (PLAYER->GetPlayer()->m_xmTransedOOBB.Intersects(m_ppObjects[0]->m_xmTransedOOBB)){
	//	//PLAYER->GetPlayer()->SetPosition(XMFLOAT3(900.0f, 900.0f, 900.0f));
	//}
	//}
	//else
	//{
	//	int i = 0;
	//}
}

void CSkinnedAnimationObjectsShader::ReleaseUploadBuffers()
{
	for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
}

void CSkinnedAnimationObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	CSkinnedAnimationShader::Render(pd3dCommandList, pCamera);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]!=NULL)
		{
			//m_ppObjects[j]->Animate(m_fElapsedTime);
			m_ppObjects[j]->UpdateTransform(NULL);
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}

	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAngrybotObjectsShader::CAngrybotObjectsShader()
{
}

CAngrybotObjectsShader::~CAngrybotObjectsShader()
{
}

void CAngrybotObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	/*int xObjects = 7, zObjects = 7, i = 0;

	m_nObjects = (xObjects * 2 + 1) * (zObjects * 2 + 1);

	m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 7.0f * 2.5f;
	float fzPitch = 7.0f * 2.5f;

	CLoadedModelInfo *pAngrybotModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Player.bin", NULL, true);

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	int nObjects = 0;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int z = -zObjects; z <= zObjects; z++)
		{
			m_ppObjects[nObjects] = new CAngrybotObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
			m_ppObjects[nObjects]->SetChild(pAngrybotModel->m_pModelRootObject, true);
			m_ppObjects[nObjects]->m_pAnimationController = new CAnimationController(1, pAngrybotModel->m_pAnimationSets);
			m_ppObjects[nObjects]->m_pAnimationController->SetTrackAnimationSet(0, (nObjects % 2));
			m_ppObjects[nObjects]->m_pAnimationController->SetTrackSpeed(0, (nObjects % 2) ? 0.25f : 1.0f);
			m_ppObjects[nObjects]->m_pAnimationController->SetTrackPosition(0, (nObjects % 3) ? 0.85f : 0.0f);
			m_ppObjects[nObjects]->m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pAngrybotModel);
			XMFLOAT3 xmf3Position = XMFLOAT3(fxPitch*x + 390.0f, 0.0f, 730.0f + fzPitch * z);
			xmf3Position.y = pTerrain->GetHeight(xmf3Position.x, xmf3Position.z);
			m_ppObjects[nObjects]->SetPosition(xmf3Position);
			m_ppObjects[nObjects++]->SetScale(2.0f, 2.0f, 2.0f);
		}
    }

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	if (pAngrybotModel) delete pAngrybotModel;*/
}

CMapObjectsShader::CMapObjectsShader()
{
}

CMapObjectsShader::~CMapObjectsShader()
{
}

void CMapObjectsShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	
}

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


	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CTexturedShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
}

D3D12_SHADER_BYTECODE CTexturedShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSTextured", "vs_5_1", ppd3dShaderBlob));

}

D3D12_SHADER_BYTECODE CTexturedShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSTextured", "ps_5_1", ppd3dShaderBlob));

}

CTexturedUIShader::CTexturedUIShader()
{
}

CTexturedUIShader::~CTexturedUIShader()
{
}

void CTexturedUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
	//CTexturedShader::BuildObjects(pd3dDevice, pd3dCommandList, pContext);

	/*m_nTexture = 1;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Gunshit.dds", 0);
	*///m_pTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//m_pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/GameMenu.dds", 0);


	
	/*CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, false);
	*/
}

void CTexturedUIShader::ReleaseObjects()
{
}

void CTexturedUIShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera, CPlayer * pPlayer)
{
	//������ ����� FrameWork�� ī�޶� ���ڷ� ���޹޴� ����, pOstProcessing�� Render�� ���� ī�޶��̴�.
	// SetViewPortsAnd,,�� �Լ��� ���� ���ÿ� 2�� ȣ���ؼ� ������ �Ǵµ� �ϴ�.
	//pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	//if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	//if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//�׸��� ����

	//if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera


	//if (m_pTexture) m_pTexture[0]->UpdateShaderVariables(pd3dCommandList);

	//pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pd3dCommandList->DrawInstanced(6, 1, 0, 0);

}

void CTexturedUIShader::CreateGraphicsRootSignature(ID3D12Device * pd3dDevice)
{
	//D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[2];

	//pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[0].NumDescriptors = 1;
	//pd3dDescriptorRanges[0].BaseShaderRegister = 19; //t19 : gtxtPilot
	//pd3dDescriptorRanges[0].RegisterSpace = 0;
	//pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;


	//pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[1].NumDescriptors = 1;
	//pd3dDescriptorRanges[1].BaseShaderRegister = 22; //t22 : gtxtPilot
	//pd3dDescriptorRanges[1].RegisterSpace = 0;
	//pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = 0;



	//D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	//pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t19: gtxtPilot
	//pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



	//pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; //t22: gtxtTarget
	//pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	//D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	//::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	//d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//d3dSamplerDesc.MipLODBias = 0;
	//d3dSamplerDesc.MaxAnisotropy = 1;
	//d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//d3dSamplerDesc.MinLOD = 0;
	//d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	//d3dSamplerDesc.ShaderRegister = 0;
	//d3dSamplerDesc.RegisterSpace = 0;
	//d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	//D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	//::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	//d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	//d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	//d3dRootSignatureDesc.NumStaticSamplers = 1;
	//d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	//d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	//ID3DBlob *pd3dSignatureBlob = NULL;
	//ID3DBlob *pd3dErrorBlob = NULL;
	//D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	////pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dGraphicsRootSignature);
	//if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	//if (pd3dErrorBlob) pd3dErrorBlob->Release();

}

D3D12_BLEND_DESC CTexturedUIShader::CreateBlendState()
{
	return D3D12_BLEND_DESC();
}

D3D12_SHADER_BYTECODE CTexturedUIShader::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTexturedUI", "vs_5_1", ppd3dShaderBlob));

}

D3D12_SHADER_BYTECODE CTexturedUIShader::CreatePixelShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTexturedUI", "ps_5_1", ppd3dShaderBlob));

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
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256�� ���
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

void CObjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Animate(fTimeElapsed);
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

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);
#endif

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera);
	}
}
//CInstancingShader::CInstancingShader()
//{
//}
//
//CInstancingShader::~CInstancingShader()
//{
//}
//D3D12_DEPTH_STENCIL_DESC CInstancingShader::CreateDepthStencilState()
//{
//	//����
//	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
//	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
//	d3dDepthStencilDesc.DepthEnable = TRUE;
//	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
//	d3dDepthStencilDesc.StencilEnable = FALSE;
//	d3dDepthStencilDesc.StencilReadMask = 0x00;
//	d3dDepthStencilDesc.StencilWriteMask = 0x00;
//	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
//	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
//
//	/*d3dDepthStencilDesc.DepthEnable = FALSE;
//	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
//	d3dDepthStencilDesc.StencilEnable = FALSE;
//	d3dDepthStencilDesc.StencilReadMask = 0x00;
//	d3dDepthStencilDesc.StencilWriteMask = 0x00;
//	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
//	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;*/
//
//	return(d3dDepthStencilDesc);
//}
//
//D3D12_BLEND_DESC CInstancingShader::CreateBlendState()
//{
//	//����
//	D3D12_BLEND_DESC d3dBlendDesc;
//	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
//	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
//	d3dBlendDesc.IndependentBlendEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
//	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
//	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
//	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
//	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	/*d3dBlendDesc.AlphaToCoverageEnable = FALSE;
//	d3dBlendDesc.IndependentBlendEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
//	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
//	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
//	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
//	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
//	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;*/
//
//	return(d3dBlendDesc);
//}
//D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
//{
//	UINT nInputElementDescs = 2;
//	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
//	//���� ������ ���� �Է� �����̴�.
//	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//
//	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
//	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
//	d3dInputLayoutDesc.NumElements = nInputElementDescs;
//	return(d3dInputLayoutDesc);
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
//
//void CInstancingShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
//{
//	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);
//
//	//�ν��Ͻ� ������ ������ ���� ���۸� ���ε� �� �������� �����Ѵ�.
//	m_pd3dcbGrassObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
//		sizeof(VS_VB_INSTANCE) * m_nGrass, D3D12_HEAP_TYPE_UPLOAD,
//		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
//	//���� ����(���ε� ��)�� ���� �����͸� �����Ѵ�.
//	m_pd3dcbGrassObjects->Map(0, NULL, (void **)&m_pcbMappedGrassObjects);
//
//	////��?�� �� �������� (���Ŀ� Ȯ�� �ٽ� �غ� ��)
//	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbTreeObjects->GetGPUVirtualAddress();
//	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
//	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nTree;
//}
//
//void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
//{
//
//
//	//pd3dCommandList->SetGraphicsRootDescriptorTable(m_pTexture->m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);
//
//	//���̺�� �����ؼ� �Ѱܾ���. 
//	for (int j = 0; j < m_nGrass; j++)
//	{
//		XMStoreFloat4x4(&m_pcbMappedGrassObjects[j].m_xmf4x4Transform,
//			XMMatrixTranspose(XMLoadFloat4x4(&m_ppGrass[j]->m_xmf4x4World)));
//	}
//	pd3dCommandList->SetGraphicsRootShaderResourceView(7, //���Ⱑ 2�ƴ϶� 3
//		m_pd3dcbGrassObjects->GetGPUVirtualAddress());
//}
//
//void CInstancingShader::ReleaseShaderVariables()
//{
//	if (m_pd3dcbGrassObjects) m_pd3dcbGrassObjects->Unmap(0, NULL);
//	if (m_pd3dcbGrassObjects) m_pd3dcbGrassObjects->Release();
//}
//
//void CInstancingShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void *pHeightMap)
//{
//	//�Ȱ�
//	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pHeightMap;
//
//	float fTerrainWidth = pTerrain->GetWidth();
//	float fTerrainLength = pTerrain->GetLength();
//
//	//������Ʈ�� ������ ���� ����ü�� �򸮴°� �ٲ�
//	int xObjects = 1;
//	int zObjects = 1;
//	m_nGrass = 1;
//
//	CTexture *ppGrassTextures;
//	ppGrassTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
//	ppGrassTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/�ð�ΰ�.dds", 0);
//
//
//	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
//
//	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1); // �ؽ��� srv
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);
//	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
//	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures, 3, true);
//	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[1], 3, true);
//	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[0], 3, true);
//	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[1], 3, true); // �� ������ �ؽ��� �ϳ��� �׷����°� �翬�ϳ�... �ᱹ �길 �Ǵ����̾���
//
//
//	CMaterial *ppGrassMaterials;
//	ppGrassMaterials = new CMaterial();
//	ppGrassMaterials->SetTexture(ppGrassTextures);
//	/*ppGrassMaterials[1] = new CMaterial();
//	ppGrassMaterials[1]->SetTexture(ppGrassTextures[1]);*/
//
//
//
//
//
//	/*CMaterial *ppFlowerMaterials[2];
//
//	ppFlowerMaterials[0] = new CMaterial();
//	ppFlowerMaterials[0]->SetTexture(ppFlowerTextures[0]);
//	ppFlowerMaterials[1] = new CMaterial();
//	ppFlowerMaterials[1]->SetTexture(ppFlowerTextures[1]);*/
//
//
//	CTexturedRectMesh *pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 5.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
//	//CTexturedRectMesh *pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 5.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f); // �������� ũ�� ����
//
//	m_ppGrass = new CGameObject*[m_nGrass];
//
//	CGrassObject *pBillboardObject = NULL;
//
//	//�򸮴� �е� ������
//	float fxPitch = 0.15f;
//	float fzPitch = 0.15f;
//
//	CMaterial *pMaterial = NULL;
//	CMesh *pMesh = NULL;
//
//	float xPosition = 0.0f; // �򸮴� ��ġ ����
//	for (int i = 0, x = 0; x < xObjects; x++)
//	{
//		xPosition += x * fxPitch;
//		float zPosition = 0.0f;
//		for (int z = 0; z < zObjects; z++)
//		{
//			pBillboardObject = new CObject();
//
//			//pBillboardObject->SetMesh(0, pFlowerMesh);
//
//			//if (x % 2) pMaterial = (z % 2) ? ppGrassMaterials[0] : ppFlowerMaterials[1];
//			//else pMaterial = (z % 2) ? ppGrassMaterials[1] : ppFlowerMaterials[0]; // z�� 0�̸� �ö�� 1dds, z�� 1�̸� �׷��� 2dds�� ���͸��� ���Ѵ�.
//			pBillboardObject->SetMaterial(ppGrassMaterials);
//
//			zPosition += z * fzPitch;
//			float fHeight = pTerrain->GetHeight(xPosition, zPosition);
//			pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 4.0f : 8.0f) * 0.5f), zPosition); // ���������� ����Ʈ�� �����ؼ� �ٴڿ��� �ڶ���� ����. Ǯ ���������� �������ִ� �κ���
//			pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
//			m_ppGrass[i++] = pBillboardObject;
//
//		}
//	}
//	m_ppGrass[0]->SetMesh(0, pGrassMesh);
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);//�̰� ���Ծ����
//
//}
//
//void CInstancingShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
//{
//	//OnPrepareRender(pd3dCommandList);
//
//	CObjectsShader::Render(pd3dCommandList, pCamera);
//	//��� ���� ��ü�� �ν��Ͻ� �����͸� ���ۿ� �����Ѵ�.
//	UpdateShaderVariables(pd3dCommandList);
//	//CTexturedShader::Render(pd3dCommandList, pCamera);
//
//
//	//�ϳ��� ���� �����͸� ����Ͽ� ��� ���� ��ü(�ν��Ͻ�)���� �������Ѵ�.
//	m_ppGrass[0]->Render(pd3dCommandList, pCamera, m_nGrass); //����
//}
//
//void CInstancingShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
//{
//	for (int i = 0; i < m_nGrass; ++i)
//	{
//		m_ppGrass[i]->Animate(fTimeElapsed, pCamera);
//	}
//}
