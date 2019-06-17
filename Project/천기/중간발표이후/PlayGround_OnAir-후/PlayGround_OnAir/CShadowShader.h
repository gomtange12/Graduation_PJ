//#pragma once
//
//class CGameObject;
//class CTexture;
//class CCamera;
//class CScene;
//class CShadowShader: public CShader
//{
//public:
//	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
//	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
//	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
//	virtual D3D12_BLEND_DESC CreateBlendState();
//
//	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
//	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera);
//	CScene*		m_pScene = nullptr;
//	CGameObject **m_ppShadow;
//	int			m_ShadowNum;
//private:
//	CTexture* m_pTexture = nullptr;
//};
//
