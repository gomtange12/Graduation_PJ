#pragma once

#include "Protocol.h"

#define PERSONNEL 2

struct stOverEx {
	WSAOVERLAPPED m_wsaOver;
	WSABUF m_wsaBuf;
	unsigned char m_IOCPbuf[MAX_BUFFER]; // IOCP send/recv 버퍼
	unsigned char	m_todo;
};
enum ThreadNum {
	ACCEPT_TH,
	WORKER_TH01,
	WORKER_TH02,
	TIMER_TH
};
enum AVATARNumber {
	A, //임시명칭
	B
};
//enum MAPNumber {
//	PLAYGROUND,
//	CONCERT
//
//};
enum ModNumber {
	SOLO,
	DUO,
	SQUAD
};
enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
enum SceneState {
	MENUSCENE = 0,
	PLAYGROUNDMAP = 1,
	CONCERTMAP,
	GAMEOVER,
	LOADING
};
enum PlayerState {
	IDLE,
	RUN,
	JUMP,
	DEATH,
	STUN,
	HIT,
	JUMPROLL,
	RUN_JUMP_ATTAK,
	KICK,
	ATTACK,
	HAPPY,
	SAD,
	BACK_RUN
};