#pragma once

const int OP_RECV = 1;
const int OP_SEND = 2;

#define CS_MOVE 2
#define SC_POSITION_INFO 3



struct stOverEx {
	WSAOVERLAPPED m_wsaOver;
	WSABUF m_wsaBuf;
	unsigned char m_IOCPbuf[MAX_BUFFER]; // IOCP send/recv ¹öÆÛ
	unsigned char	m_todo;
};


//////////////////////packet/////////////////////////
struct move_packet
{
	unsigned char size;
	unsigned char type;
};

struct sc_packet_pos {//SC_POSITION_INFO 3 
	unsigned char size;
	unsigned char type;
	WORD ID;
	WORD X_POS;
	WORD Y_POS;
};