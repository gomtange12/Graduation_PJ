#pragma once
#include "Player.h"
//class CPlayer;
class COtherPlayers : public CPlayer
{
public:
	COtherPlayers(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, E_CHARACTERTYPE type, void *pContext = NULL);
	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	virtual void OnCameraUpdateCallback(float fTimeElapsed);
	virtual void Update(float fTimeElapsed);
	virtual ~COtherPlayers(){}
public:

};
