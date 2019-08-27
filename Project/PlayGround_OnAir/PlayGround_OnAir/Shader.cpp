//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "CNetWork.h"
#include "Player.h"
#include "CPlayerManager.h"
#include "CSceneManager.h"

#include "CBillboardObject.h"
#include "Scene.h"
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
	if (pd3dErrorBlob)
	{
		pErrorString = (char *)pd3dErrorBlob->GetBufferPointer();
		wchar_t pwstrDest[100000];
		int nLen = (int)strlen(pErrorString) + 1;
		mbstowcs(pwstrDest, pErrorString, nLen);
		//MessageBox(NULL, pwstrDest, L"System Message", MB_OK);
	}
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
	//int xObjects = 1, zObjects = 1, i = 0;

	//m_nObjects = 1;// (xObjects * 2 + 1) * (zObjects * 2 + 1);

	//m_ppObjects = new CGameObject*[m_nObjects];

	//float fxPitch = 7.0f * 2.5f;
	//float fzPitch = 7.0f * 2.5f;

	//CLoadedModelInfo *pAngrybotModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);

	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	//int nObjects = 0;
	///*for (int x = -xObjects; x <= xObjects; x++)
	//{
	//	for (int z = -zObjects; z <= zObjects; z++)
	//	{*/
	//		m_ppObjects[nObjects] = new CAngrybotObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	//		m_ppObjects[nObjects]->SetChild(pAngrybotModel->m_pModelRootObject, true);
	//		m_ppObjects[nObjects]->m_pAnimationController = new CAnimationController(1, pAngrybotModel->m_pAnimationSets);
	//		m_ppObjects[nObjects]->m_pAnimationController->SetTrackAnimationSet(0, (nObjects % 2));
	//		m_ppObjects[nObjects]->m_pAnimationController->SetTrackSpeed(0, (nObjects % 2) ? 0.25f : 1.0f);
	//		m_ppObjects[nObjects]->m_pAnimationController->SetTrackPosition(0, (nObjects % 3) ? 0.85f : 0.0f);
	//		m_ppObjects[nObjects]->m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pAngrybotModel);
	//		
	//		m_ppObjects[nObjects]->SetPosition(XMFLOAT3(40,0,0));
	//		m_ppObjects[nObjects++]->SetScale(2.0f, 2.0f, 2.0f);
	//	

	//CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//if (pAngrybotModel) delete pAngrybotModel;
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

void CTexturedShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pd3dcbUIObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(UI_Data), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbUIObjects->Map(0, NULL, (void **)&m_pcbMappedUIObjects);
}

void CTexturedShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{

	XMStoreFloat4x4(&m_pcbMappedUIObjects->m_xmf4x4Transform,
		XMMatrixTranspose(XMLoadFloat4x4(&m_ppUIObjects[0]->m_xmf4x4World)));
	//XMStoreFloat2(&m_pcbMappedUIObjects->m_uvCoord,XMLoadFloat2(m_ppUIObjects[0]->m_pMesh->))

	pd3dCommandList->SetGraphicsRootShaderResourceView(10, //여기가 2아니라 3
		m_pd3dcbUIObjects->GetGPUVirtualAddress());
}

void CTexturedShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	
}

void CTexturedShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	CShader::Render(pd3dCommandList, pCamera);
}



D3D12_BLEND_DESC CTexturedShader::CreateBlendState()
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

	return d3dBlendDesc;

}

D3D12_DEPTH_STENCIL_DESC CTexturedShader::CreateDepthStencilState()
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
	//릴리즈 실행시 FrameWork의 카메라를 인자로 전달받는 도중, pOstProcessing의 Render와 같은 카메라이다.
	// SetViewPortsAnd,,이 함수를 거의 동시에 2번 호출해서 문제가 되는듯 하다.
	//pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	//if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	//if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	//if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera


	//if (m_pTexture) m_pTexture[0]->UpdateShaderVariables(pd3dCommandList);

	//pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pd3dCommandList->DrawInstanced(6, 1, 0, 0);

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

void CUiShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{


	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	
	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/LobbyUI/menu2.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);
	

}

D3D12_SHADER_BYTECODE CUiShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSUITextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}

D3D12_SHADER_BYTECODE CUiShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSUITextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}


void CUiShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	m_pTexture->UpdateShaderVariables(pd3dCommandList);
	CShader::Render(pd3dCommandList, pCamera);

	pd3dCommandList->DrawInstanced(6, 1, 0, 0);
}

D3D12_SHADER_BYTECODE CBillBoardShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSUITextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}

D3D12_SHADER_BYTECODE CBillBoardShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSUITextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}

void CBillBoardShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_ppBillBoardObj = new CGameObject*[m_BillboardNum];
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/LobbyUI/menu.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);
}

void CBillBoardShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	m_pTexture->UpdateShaderVariables(pd3dCommandList);
	CShader::Render(pd3dCommandList, pCamera);

	pd3dCommandList->DrawInstanced(6, 1, 0, 0);
}


D3D12_SHADER_BYTECODE CUIPlayerShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSPlayerUITextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}
D3D12_SHADER_BYTECODE CUIPlayerShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSPlayerUITextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}

void CUIPlayerShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	
	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/여_키타_L.dds", 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/LobbyUI/메뉴씬.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}

void CUIOtherPlayerShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/남R_기타_L.dds", 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/LobbyUI/메뉴씬.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}
D3D12_SHADER_BYTECODE CUIOtherPlayerShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSOtherPlayerUITextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}
D3D12_SHADER_BYTECODE CUIOtherPlayerShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSOtherPlayerUITextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}
void CUISKillShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/cooldown.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}
D3D12_SHADER_BYTECODE CUISKillShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSSSkillTextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}
D3D12_SHADER_BYTECODE CUISKillShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSSkillUITextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}
void CTimeBarShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/timeBar.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}
D3D12_SHADER_BYTECODE CTimeBarShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSSTimeBarTextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}
D3D12_SHADER_BYTECODE CTimeBarShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSTimeBarTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}
D3D12_SHADER_BYTECODE CWinUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSWinTextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}

D3D12_SHADER_BYTECODE CWinUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSWinTextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}

void CWinUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/win.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}
D3D12_SHADER_BYTECODE CLoseUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSLoseTextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}

D3D12_SHADER_BYTECODE CLoseUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSLoseTextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}

void CLoseUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/lose.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}
CHPUIShader::CHPUIShader()
{
}

CHPUIShader::~CHPUIShader()
{
}

void CHPUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{

	m_nObjects = 1;
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/heart.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);


	m_cbHp = new CB_HP_INFO;
	::ZeroMemory(m_cbHp, sizeof(CB_HP_INFO));

	m_cbHp->hp = 8;
}

D3D12_SHADER_BYTECODE CHPUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSHPTextured", "ps_5_1", &m_pd3dPixelShaderBlob));
}

D3D12_SHADER_BYTECODE CHPUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSHPTextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}

void CHPUIShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_HP_INFO) + 255) & ~255); //256의 배수
	m_cbHPResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_cbHPResouce->Map(0, NULL, (void **)&m_cbMappedHp);

}

void CHPUIShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{

	m_cbHp->hp = PLAYER->GetPlayer()->GetHP() - 1 ;
	if (m_cbHp->hp < 1)
		PLAYER->GetPlayer()->SetHeartRender(false);
	//cout << "hp: " << m_cbHp->hp << endl;
	UINT ncbElementBytes = ((sizeof(CB_HP_INFO) + 255) & ~255);

	CB_HP_INFO *pbMappedcbHPInfo = (CB_HP_INFO *)((UINT8 *)m_cbMappedHp + (0 * ncbElementBytes));
	::memcpy(m_cbMappedHp, m_cbHp, sizeof(CB_HP_INFO));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbHPResouce->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(18, d3dGpuVirtualAddress);

}

void CHPUIShader::ReleaseShaderVariables()
{
	if (m_cbHPResouce)
	{
		m_cbHPResouce->Unmap(0, NULL);
		m_cbHPResouce->Release();
	}

}


D3D12_SHADER_BYTECODE CChatUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSChatTextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}

D3D12_SHADER_BYTECODE CChatUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSChatTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}

void CChatUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{

	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/BG.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

}

D3D12_BLEND_DESC CChatUIShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_COLOR;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return d3dBlendDesc;
}






D3D12_SHADER_BYTECODE CSkillCoolDownUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSSkillCoolTextured", "ps_5_1", &m_pd3dPixelShaderBlob));
}

D3D12_SHADER_BYTECODE CSkillCoolDownUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSSkillCoolTextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}
void CSkillCoolDownUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/cooldown.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

	m_cbSkillCool = new CB_SKILL_INFO;
	::ZeroMemory(m_cbSkillCool, sizeof(CB_SKILL_INFO));

	m_cbSkillCool->Cooldown = 0;
}

void CSkillCoolDownUIShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_SKILL_INFO) + 255) & ~255); //256의 배수
	m_cbResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_cbResouce->Map(0, NULL, (void **)&m_cbMappedSkillCool);
}

void CSkillCoolDownUIShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	
	m_cbSkillCool->Cooldown = CNETWORK->GetSkillTime();// PLAYER->GetPlayer()->GetSkillCount();
	//cout << "updateshaderVar: " << m_cbSkillCool->Cooldown << endl;
	UINT ncbElementBytes = ((sizeof(CB_SKILL_INFO) + 255) & ~255);

	CB_SKILL_INFO *pbMappedcbSkillInfo = (CB_SKILL_INFO *)((UINT8 *)m_cbMappedSkillCool + (0 * ncbElementBytes));
	::memcpy(m_cbMappedSkillCool, m_cbSkillCool, sizeof(CB_SKILL_INFO));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbResouce->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(19, d3dGpuVirtualAddress);

}

void CSkillCoolDownUIShader::ReleaseShaderVariables()
{
	if (m_cbResouce)
	{
		m_cbResouce->Unmap(0, NULL);
		m_cbResouce->Release();
	}

}



D3D12_SHADER_BYTECODE CSkillEffectUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSEffectTextured", "ps_5_1", &m_pd3dPixelShaderBlob));
}

D3D12_SHADER_BYTECODE CSkillEffectUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSEffectTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}
//
//void CSkillEffectUIShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
//{
//	m_cbSprite = new CB_SPRITE_TIME;
//	::ZeroMemory(m_cbSprite, sizeof(CB_SPRITE_TIME));
//
//	m_cbSprite->xPos = 0;
//
//	UINT ncbElementBytes = ((sizeof(CB_SPRITE_TIME) + 255) & ~255); //256의 배수
//	m_cbResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//
//	m_cbResouce->Map(0, NULL, (void **)&m_cbMappedSprite);
//}

//void CSkillEffectUIShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
//{
//	m_cbSprite->xPos = 2;// PLAYER->GetPlayer()->GetSkillCount();
//	//cout << "ten"<< m_cbClock->tenSec << endl;
////cout << "updateshaderVar: " << m_cbSkillCool->Cooldown << endl;
//	UINT ncbElementBytes = ((sizeof(CB_SPRITE_TIME) + 255) & ~255);
//
//	CB_SPRITE_TIME *pbMappedcbSkillInfo = (CB_SPRITE_TIME *)((UINT8 *)m_cbMappedSprite + (ncbElementBytes));
//	::memcpy(m_cbMappedSprite, m_cbSprite, sizeof(CB_SPRITE_TIME));
//
//	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbResouce->GetGPUVirtualAddress();// +m_ObjectCBIndex * ncbElementBytes;
//	pd3dCommandList->SetGraphicsRootConstantBufferView(21, d3dGpuVirtualAddress);
//
//
//}

D3D12_INPUT_LAYOUT_DESC CSkillEffectUIShader::CreateInputLayout()
{

	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}



//player skill
D3D12_SHADER_BYTECODE CPlayerEffectUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSEffectTextured", "ps_5_1", &m_pd3dPixelShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerEffectUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSEffectTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_INPUT_LAYOUT_DESC CPlayerEffectUIShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	//두개 넘길거면 두번쨰 인자를 0, 1로 인덱스 구분 아니면 네번째 인자로 구분하면 된다. 
	//저번에 안넘어간 이유는 버퍼를 만들고 값을 채우지 않았기 떄문
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}


void CPlayerEffectUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature,E_EFFECTTYPE type,  void * pContext)
{
	CTexture* pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/basicSkill.dds", 0);

	m_nObjects = 1;
	CTexturedRectMesh* pEffectMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20, 20.f, 0, 0, 0, 0);


	m_pMaterial = new CMaterial(1);
	m_pMaterial->SetTexture(pTexture, 0);

	m_ppObjects = new CGameObject*[m_nObjects];


	m_ppObjects[0] = new CPlaneObject(1);
	m_ppObjects[0]->SetMesh(pEffectMesh);
	m_ppObjects[0]->SetMaterial(0, m_pMaterial);
	m_ppObjects[0]->SetPosition(350, 10, 1745);
	m_ppObjects[0]->SetScale(30, 30, 30);
	//pPlaneObject->SetScale(100, 100, 100);

	CScene::CreateShaderResourceViews(pd3dDevice, pTexture, 16, false);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

}


void CPlayerEffectUIShader::ReleaseShaderVariables()
{

}

void CPlayerEffectUIShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dCommandList, 0);
	CShader::Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_nObjects; ++i)
		m_ppObjects[i]->Render(pd3dCommandList, pCamera);
	
}

D3D12_SHADER_BYTECODE CTenSecShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSTenSecTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}

void CTenSecShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/numbers.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

	m_cbMappeClock = new CB_TEMP_CLOCK_INFO;
	::ZeroMemory(m_cbMappeClock, sizeof(CB_TEMP_CLOCK_INFO));

	m_cbMappeClock->tenSec = 3;
}



void CTenSecShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	n = SCENEMANAGER->GetTenSec();
	UINT ncbElementBytes = ((sizeof(CB_TEMP_CLOCK_INFO) + 255) & ~255);

	::memcpy(&m_cbMappeClock->tenSec, &n, sizeof(int));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbClockResouce->GetGPUVirtualAddress();// +3 * ncbElementBytes;
   pd3dCommandList->SetGraphicsRootConstantBufferView(13, d3dGpuVirtualAddress);
}


void CTenSecShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_TEMP_CLOCK_INFO) + 255) & ~255); //256의 배수
	m_cbClockResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_cbClockResouce->Map(0, NULL, (void **)&m_cbMappeClock);
}
D3D12_SHADER_BYTECODE COneSecUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSOneSecTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}

void COneSecUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/numbers.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

	m_cbMappeClock = new CB_TEMP_CLOCK_INFO;
	::ZeroMemory(m_cbMappeClock, sizeof(CB_TEMP_CLOCK_INFO));
}

void COneSecUIShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	
	n = SCENEMANAGER->GetOneSec();
	UINT ncbElementBytes = ((sizeof(CB_TEMP_CLOCK_INFO) + 255) & ~255);

	::memcpy(&m_cbMappeClock->oneSec, &n, sizeof(int));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbClockResouce->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(13, d3dGpuVirtualAddress);


}

void COneSecUIShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_TEMP_CLOCK_INFO) + 255) & ~255); //256의 배수
	m_cbClockResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_cbClockResouce->Map(0, NULL, (void **)&m_cbMappeClock);
}

D3D12_SHADER_BYTECODE CTimerUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSClockTextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}

D3D12_SHADER_BYTECODE CTimerUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSHundredSecTextured", "vs_5_1", &m_pd3dVertexShaderBlob));
}

void CTimerUIShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_TEMP_CLOCK_INFO) + 255) & ~255); //256의 배수
	m_cbClockResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_cbClockResouce->Map(0, NULL, (void **)&m_cbMappeClock);
}

void CTimerUIShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	n = SCENEMANAGER->GetHunSec();
	UINT ncbElementBytes = ((sizeof(CB_TEMP_CLOCK_INFO) + 255) & ~255);

	::memcpy(&m_cbMappeClock->hunSec, &n, sizeof(int));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbClockResouce->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(13, d3dGpuVirtualAddress);

}

void CTimerUIShader::ReleaseShaderVariables()
{
	if (m_cbClockResouce)
	{
		m_cbClockResouce->Unmap(0, NULL);
		m_cbClockResouce->Release();
	}
}

void CTimerUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/numbers.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);

	m_cbMappeClock = new CB_TEMP_CLOCK_INFO;
	::ZeroMemory(m_cbMappeClock, sizeof(CB_TEMP_CLOCK_INFO));

}

void CESkillEffectShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_SPRITE_TIME) + 255) & ~255); //256의 배수
	m_cbResouce = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_cbResouce->Map(0, NULL, (void **)&m_cbMappedSprite);
}

void CESkillEffectShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_cbSprite->xPos = 3;
	m_cbSprite->yPos = 3;

	UINT ncbElementBytes = ((sizeof(CB_SPRITE_TIME) + 255) & ~255);

	CB_SPRITE_TIME *pbMappedcbSprite = (CB_SPRITE_TIME *)((UINT8 *)m_cbMappedSprite);// +(i * ncbElementBytes));
	::memcpy(m_cbMappedSprite, m_cbSprite, sizeof(CB_SPRITE_TIME));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_cbResouce->GetGPUVirtualAddress();// +Index * ncbElementBytes;
	pd3dCommandList->SetGraphicsRootConstantBufferView(13, d3dGpuVirtualAddress);

}
