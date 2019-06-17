#pragma once
#include "Shader.h"

class CGameObject;
class CTexture;
class CCamera;

class CInstancingShader : public CTexturedShader
{
public:
	CInstancingShader();
	~CInstancingShader();

public:


	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
		*pd3dGraphicsRootSignature);
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList, void* pHeightMap);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual	void AnimateObjects(float fTimeElapsed, CCamera* pCamera);

	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

protected:
	int m_nGrass;
	CGameObject	  **m_ppUI = 0;
	CTexture *m_pTexture;
	//인스턴스 데이터를 포함하는 버퍼와 포인터이다.
	//정점 버퍼와 정점버퍼뷰
	ID3D12Resource * m_pd3dcbUIObjects = NULL;
	VS_VB_INSTANCE *m_pcbMappedUIObjects = NULL;

	////인스턴스 데이터를 포함하는 버퍼와 포인터이다.
	/*ID3D12Resource *m_pd3dcbGameObjects = NULL;
	VS_VB_INSTANCE *m_pcbMappedGameObjects = NULL;*/


	//D3D12_VERTEX_BUFFER_VIEW m_d3dInstancingBufferView;
};

