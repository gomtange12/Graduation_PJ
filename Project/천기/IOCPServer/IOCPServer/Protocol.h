#pragma once

#define MAX_BUFFER		1024
#define SERVER_PORT		3500
#define MAX_USER		10

const int OP_RECV = 1;
const int OP_SEND = 2;

////////////////////
#define CS_MOVE 2
#define SC_POSITION_INFO 3

////////////////////

////=확인용=/////
constexpr int WORLD_WIDTH = 8;
constexpr int WORLD_HEIGHT = 8;
////=확인용=/////

constexpr int SC_LOGIN_OK = 1;
constexpr int SC_PUT_PLAYER = 2;
constexpr int SC_REMOVE_PLAYER = 3;
constexpr int SC_MOVE_PLAYER = 4;
constexpr int SC_MATCHING_PLAYER = 5;
constexpr int SC_MATCHING_RESULT = 6;

struct stOverEx {
	WSAOVERLAPPED m_wsaOver;
	WSABUF m_wsaBuf;
	unsigned char m_IOCPbuf[MAX_BUFFER]; // IOCP send/recv 버퍼
	unsigned char	m_todo;
};

//////////////////////packet/////////////////////////
struct sc_packet_pos {//SC_POSITION_INFO 3 
	unsigned char size;
	unsigned char type;
	WORD ID;
	WORD X_POS;
	WORD Y_POS;
};
struct sc_packet_login_ok {
	unsigned char size;
	unsigned char type;
	char id;
};
struct sc_packet_put_player {
	unsigned char size;
	unsigned char type;
	char id;
	char x, y;
};
struct sc_packet_remove_player {
	unsigned char size;
	unsigned char type;
	char id;
};
struct sc_packet_matching {
	unsigned char size;
	unsigned char type;
	char map;
	char charac;
	char mod;
};
struct sc_packet_match_result {
	unsigned char size;
	unsigned char type;
	char result;
};