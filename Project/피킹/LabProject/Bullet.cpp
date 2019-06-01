#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet(XMFLOAT3 pos,XMFLOAT3 right, XMFLOAT3 up, XMFLOAT3 look, XMFLOAT3 velocity, XMFLOAT3 movingdirection)
	: m_xmf3Right(right), m_xmf3Up(up), m_xmf3Look(look), m_xmf3Velocity(velocity)
{

	CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

	SetPosition(pos);
	SetMesh(pObjectCubeMesh);
	SetColor(RGB(100, 100, 0));
	SetRotationAxis(up);
	SetRotationSpeed(200);

	SetMovingDirection(up);
	SetMovingSpeed(100);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

}
CBullet::~CBullet()
{
}
