#pragma once
#include "pch.h"
#include "Protocol.h"

class Player
{
public:
	SOCKET m_socket;
	stOverEx m_RecvOverEx;
	unsigned char m_packet_buf[MAX_BUFFER];
	int m_prev_size;

	wchar_t m_name[20];
	BYTE m_x;
	BYTE m_y;
	bool m_connected;
	int m_id;
public:
	Player();
	~Player();

private:

};

