#pragma once

#include "Player.h"
#include "Bullet.h"

class CPlayer;
class CBullet : public CGameObject
{
public:
	CBullet(XMFLOAT3 pos,XMFLOAT3 right, XMFLOAT3 up, XMFLOAT3 look, XMFLOAT3 velocity, XMFLOAT3);
	~CBullet();
	

public:
	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	XMFLOAT3					m_xmf3CameraOffset;
	XMFLOAT3					m_xmf3Velocity;
	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;
	CPlayer						*m_pPlayer;
};

