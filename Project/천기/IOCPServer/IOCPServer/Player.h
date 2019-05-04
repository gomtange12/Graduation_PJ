#pragma once
#include "pch.h"
#include "Protocol.h"
#include "Functor.h"
#include "GameObject.h"

struct stOverEx {
	WSAOVERLAPPED m_wsaOver;
	WSABUF m_wsaBuf;
	unsigned char m_IOCPbuf[MAX_BUFFER]; // IOCP send/recv 버퍼
	unsigned char	m_todo;
};
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
	int roomNumber;
public:
	XMFLOAT3					m_xmf3Position  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Look;// = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMFLOAT3					m_xmf3Right;// = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up;// = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//bool ready;
	int avatar;
	int mod;
	int map;
	int state;


public:
	Player();
	~Player();

	void Initialize();

};

