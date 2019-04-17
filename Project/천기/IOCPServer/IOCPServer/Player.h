#pragma once
#include "pch.h"
#include "Protocol.h"

struct stOverEx {
	WSAOVERLAPPED m_wsaOver;
	WSABUF m_wsaBuf;
	unsigned char m_IOCPbuf[MAX_BUFFER]; // IOCP send/recv ¹öÆÛ
	unsigned char	m_todo;
};

class Player
{
public: //
	SOCKET m_socket;
	stOverEx m_RecvOverEx;
	unsigned char m_packet_buf[MAX_BUFFER];
	int m_prev_size;

public:
	bool m_connected;
	bool m_join;
	int m_id;

public:
	wchar_t m_name[20];
	BYTE m_x;
	BYTE m_y;
	bool ready=false;
	int avatar;
	int mod;
	int map;
	//DirectX::XMFLOAT3	m_xmf3Position;


public:
	Player();
	~Player();

	void Initialize();

};

