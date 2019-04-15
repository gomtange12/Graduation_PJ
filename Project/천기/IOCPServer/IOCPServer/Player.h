#pragma once
#include "pch.h"
#include "Protocol.h"

class Player
{
public:
	bool m_connected;
	bool m_join;
	int m_id;

	wchar_t m_name[20];
	BYTE m_x;
	BYTE m_y;

public: //
	SOCKET m_socket;
	stOverEx m_RecvOverEx;
	unsigned char m_packet_buf[MAX_BUFFER];
	int m_prev_size;


public:
	Player();
	~Player();

	void Initialize();

};

