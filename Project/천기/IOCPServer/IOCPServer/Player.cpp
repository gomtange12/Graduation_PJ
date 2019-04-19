#include "pch.h"
#include "Player.h"

Player::Player()
{
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

	m_x=4;
	m_y=4;
}