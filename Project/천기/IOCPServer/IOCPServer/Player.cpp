#include "pch.h"
#include "Player.h"

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

	m_xmf4x4ToParent._21 = m_xmf3Look.x; 
	m_xmf4x4ToParent._22 = m_xmf3Look.y; 
	m_xmf4x4ToParent._23 = m_xmf3Look.z;

	
}