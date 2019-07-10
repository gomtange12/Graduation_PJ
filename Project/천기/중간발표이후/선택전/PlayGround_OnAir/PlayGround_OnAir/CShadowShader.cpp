#include "stdafx.h"
//#include "Shader.h"
//#include "CShadowShader.h"
//#include "Object.h"
//#include "CSceneManager.h"
//#include "Scene.h"
//D3D12_SHADER_BYTECODE CShadowShader::CreateVertexShader()
//{
//	return D3D12_SHADER_BYTECODE();
//}
//
//D3D12_SHADER_BYTECODE CShadowShader::CreatePixelShader()
//{
//	return D3D12_SHADER_BYTECODE();
//}
//
//D3D12_INPUT_LAYOUT_DESC CShadowShader::CreateInputLayout()
//{
//	UINT nInputElementDescs = 1;
//	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
//
//	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//
//	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
//	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
//	d3dInputLayoutDesc.NumElements = nInputElementDescs;
//
//	return(d3dInputLayoutDesc);
//	
//}
//
//D3D12_BLEND_DESC CShadowShader::CreateBlendState()
//{
//	D3D12_BLEND_DESC d3dBlendDesc;
//	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
//	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
//	d3dBlendDesc.IndependentBlendEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
//	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
//	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
//	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
//	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
//	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	return(d3dBlendDesc);
//}
//
//void CShadowShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
//{
//}
//
//void CShadowShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
//{
//	CShader::Render(pd3dCommandList, pCamera);
//	XMFLOAT4 xmLight(0.0f, 35.0f, 0.0f, 1.0f);
//	XMFLOAT4 xmPlane(0.0f, 1.0f, 0.0f, 10.0f);
//
//	switch (SCENEMANAGER->GetSceneType())
//	{
//	case PLAYGROUNDMAP:
//		for (int i = 0; i < m_pScene->m_nPlayGroundObjects; ++i)
//		{
//			XMMATRIX xmmtxPlane = XMMatrixShadow(XMLoadFloat4(&xmPlane), XMLoadFloat4(&xmLight));
//			XMFLOAT4X4 xmf4x4PlaneProject= Matrix4x4::Multiply(xmmtxPlane,m_pScene->m_ppPlayGroundObjects[i]->m_xmf4x4World);
//			m_pScene->m_ppPlayGroundObjects[i]->UpdateShaderVariable(pd3dCommandList, &xmf4x4PlaneProject);
//		}
//		break;
//	case CONCERTMAP:
//		break;
//	}
//	//for(int i = 0; i<m_pscene)
//}
//
//
