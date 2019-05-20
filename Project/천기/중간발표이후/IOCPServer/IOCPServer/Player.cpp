#include "pch.h"
#include "Player.h"
#include "Functor.h"

Player::Player()
{
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

Player::~Player()
{
}
void Player::Initialize() {
	m_join = false;
	m_connected = false;
	//ready = false;
	m_match = false;

	m_RecvOverEx.m_todo = OP_RECV;
	m_RecvOverEx.m_wsaBuf.buf = (CHAR*)m_RecvOverEx.m_IOCPbuf;
	m_RecvOverEx.m_wsaBuf.len = sizeof(m_RecvOverEx.m_IOCPbuf);
	m_prev_size = 0;

	m_xmf4x4ToParent._11 = m_xmf3Right.x;
	m_xmf4x4ToParent._12 = m_xmf3Right.y;
	m_xmf4x4ToParent._13 = m_xmf3Right.z;

	m_xmf4x4ToParent._21 = m_xmf3Up.x;
	m_xmf4x4ToParent._22 = m_xmf3Up.y;
	m_xmf4x4ToParent._23 = m_xmf3Up.z;

	m_xmf4x4ToParent._31 = m_xmf3Look.x; 
	m_xmf4x4ToParent._32 = m_xmf3Look.y; 
	m_xmf4x4ToParent._33 = m_xmf3Look.z;

	m_xmf4x4ToParent._41 = m_xmf3Position.x;
	m_xmf4x4ToParent._42 = m_xmf3Position.y;
	m_xmf4x4ToParent._43 = m_xmf3Position.z;
}
void Player::move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity) 
{
	if (this != nullptr)
	{
		if (bUpdateVelocity)
		{
			m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
		}
		else
		{
			m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
		
		}
	}
}
void Player::Transform() {
	m_xmf4x4ToParent._11 = m_xmf3Right.x; m_xmf4x4ToParent._12 = m_xmf3Right.y; m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x; m_xmf4x4ToParent._22 = m_xmf3Up.y; m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x; m_xmf4x4ToParent._32 = m_xmf3Look.y; m_xmf4x4ToParent._33 = m_xmf3Look.z;
	m_xmf4x4ToParent._41 = m_xmf3Position.x; m_xmf4x4ToParent._42 = m_xmf3Position.y; m_xmf4x4ToParent._43 = m_xmf3Position.z;

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);
	
	XMFLOAT4X4 *pxmf4x4Parent = NULL;
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4ToParent, *pxmf4x4Parent) : m_xmf4x4ToParent;
}
