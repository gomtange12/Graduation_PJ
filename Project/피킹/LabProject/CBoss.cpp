#include "stdafx.h"
#include "CBoss.h"


CBoss::CBoss(XMFLOAT3 pos)
{
	CCubeMesh *pObjectCubeMesh = new CCubeMesh(40.0f, 20.0f, 20.0f);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(20.0f, 10.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetPosition(0, 0, 450);
	SetMesh(pObjectCubeMesh);
	SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetRotationSpeed(160.0f);
	//SetMovingDirection(XMFLOAT3(Vector3::Subtract(pos, GetPosition())));
	//SetMovingRange(50);
	if (GetPosition().z - pos.z<100) //플레이어가 100미터 이내로 접근하면 플레이어에게 접근한다
		SetMovingSpeed(20);
	SetColor(RGB(0, 0, 0));
	//cout << GetPosition().z << endl;
}


CBoss::~CBoss()
{
}
