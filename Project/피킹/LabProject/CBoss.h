#pragma once
#include "GameObject.h"
#include "Bullet.h"
class CBullet;

class CBoss : public CGameObject
{
public:
	CBoss(XMFLOAT3 pos);
	~CBoss();
	CBullet						*m_pBossBullet = NULL;
	vector<CBullet*>			m_vecBossBullet;

};

