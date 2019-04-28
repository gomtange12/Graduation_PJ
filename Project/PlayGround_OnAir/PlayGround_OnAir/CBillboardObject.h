#pragma once
#include "stdafx.h"
#include "Object.h"
class CMaterial;
class CBillboardObject : public CGameObject
{
public:
	CBillboardObject();
	~CBillboardObject();

	public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera = NULL);
	virtual void Animate(float fTimeElapsed, std::shared_ptr<CCamera>);

	virtual void SetLookAt(XMFLOAT3& xmfTarget);
};

class UIBillboardObject : public CBillboardObject
{
public:
	UIBillboardObject();
	virtual ~UIBillboardObject();

	//void SetLookAt(XMFLOAT3& xmfTarget);

	virtual void Animate(float fElapsedTim, std::shared_ptr<CCamera>);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera = NULL);

};

