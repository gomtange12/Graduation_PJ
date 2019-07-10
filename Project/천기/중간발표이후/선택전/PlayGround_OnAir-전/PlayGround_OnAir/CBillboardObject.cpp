#include "stdafx.h"
#include "CBillboardObject.h"
#include "Shader.h"
#include "Scene.h"

CBillboardObject::CBillboardObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature) : CGameObject(1)
{

	/*CTexturedRectMesh *pRectMesh = new  CTexturedRectMesh(pd3dDevice, pd3dCommandList, 100.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	SetMesh(pRectMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	CTexture *ppUITextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//ppUITextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"시계로고.dds", 0);
	ppUITextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/pg_logo.dds", 0);


	/*CTexture *ppUITextures2 = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppUITextures2->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Leader.dds", 0);
* /

	CTexturedShader *pUIShader = new CTexturedShader();
	pUIShader->CreateShader(pd3dDevice,pd3dCommandList, pd3dGraphicsRootSignature);
	pUIShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pUIShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, NULL);

	CScene::CreateShaderResourceViews(pd3dDevice, ppUITextures, 14, false);
	//CScene::CreateShaderResourceViews(pd3dDevice, ppUITextures2, 14, false);
	CMaterial *pUIMaterial = new CMaterial(1);
	pUIMaterial->SetTexture(ppUITextures, 0);
	//pUIMaterial->SetTexture(ppUITextures2, 1);
	pUIMaterial->SetShader(pUIShader);

	SetMaterial(0, pUIMaterial);*/
	//SetMaterial(, pUIMaterial);


	//SetPosition(PLAYER->GetPlayer()->GetPosition());
}


CBillboardObject::~CBillboardObject()
{
}

void CBillboardObject::Animate(float fTimeElapsed, std::shared_ptr<CCamera>  pCamera)
{
	XMFLOAT3 m_xmf3CameraPosition = pCamera->GetPosition();
	SetLookAt(m_xmf3CameraPosition);
}

void CBillboardObject::UpdateShaderVariable(ID3D12GraphicsCommandList * pd3dCommandList)
{

}

void CBillboardObject::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera )
{
	
	CGameObject::Render(pd3dCommandList, pCamera);
}

void CBillboardObject::SetLookAt(XMFLOAT3 & xmfTarget)
{
	XMFLOAT3 m_xmf3Up(0.0f, 1.0f, 0.0f);
	XMFLOAT3 m_xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMFLOAT3 m_xmf3Look = Vector3::SubtractNormalize(xmfTarget, m_xmf3Position, true);
	XMFLOAT3 m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);

	m_xmf4x4World._11 = m_xmf3Right.x;
	m_xmf4x4World._12 = m_xmf3Right.y;
	m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x;
	m_xmf4x4World._22 = m_xmf3Up.y;
	m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x;
	m_xmf4x4World._32 = m_xmf3Look.y;
	m_xmf4x4World._33 = m_xmf3Look.z;
}

UIBillboardObject::UIBillboardObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{

}

UIBillboardObject::~UIBillboardObject()
{
}

void UIBillboardObject::Animate(float fElapsedTime, std::shared_ptr<CCamera> pCamera)
{
	CBillboardObject::Animate(fElapsedTime, pCamera);
	XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z - 1);
}

void UIBillboardObject::Render(ID3D12GraphicsCommandList * pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender();
	CGameObject::Render(pd3dCommandList, pCamera);
}
