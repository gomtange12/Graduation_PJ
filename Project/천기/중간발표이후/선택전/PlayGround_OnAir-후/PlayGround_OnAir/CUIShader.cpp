#include "stdafx.h"
#include "CUIShader.h"
#include "Scene.h"


void CUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{


	m_pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/cbka0-bdgu5.dds", 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/LobbyUI/menu.dds", 0);

	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/LobbyUI/메뉴씬.dds", 0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, false);


}

D3D12_SHADER_BYTECODE CUIShader::CreateVertexShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "VSUITextured", "vs_5_1", &m_pd3dVertexShaderBlob));

}

D3D12_SHADER_BYTECODE CUIShader::CreatePixelShader()
{
	return(CShader::CompileShaderFromFile(L"UIShader.hlsl", "PSUITextured", "ps_5_1", &m_pd3dPixelShaderBlob));

}


void CUIShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
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
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/InGameUI/스킬_ON.dds", 0);

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
