#pragma once
#include "pch.h"
#include "Protocol.h"
#include "Functor.h"
#include "GameObject.h"
#include "Define.h"

////////////////////////////////
class Player : public GameObject
{
public: //
	SOCKET m_socket;
	stOverEx m_RecvOverEx;
	unsigned char m_packet_buf[MAX_BUFFER];
	int m_prev_size;

public:
	bool m_connected; //연결상태
	bool m_join; //리조인확인용
	bool m_match; //매칭확인용
	
	int m_id;
	int roomNumber=0;
	
public:
	XMFLOAT3					m_xmf3Position  = XMFLOAT3(-530, 50, 745);
	XMFLOAT3					m_xmf3Look;// = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMFLOAT3					m_xmf3Right;// = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up;// = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	float           			m_fMaxVelocityXZ = 0.f;
	float           			m_fMaxVelocityY = 0.f;
	float           			m_fFriction = 0.0f;

	//bool ready;
	int avatar;
	int mod;
	int map;
	int state;

	BoundingOrientedBox			m_xmOOBB;
public:
	Player();
	~Player();

	void Initialize();
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }
	void move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Transform();
	
	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }

};

