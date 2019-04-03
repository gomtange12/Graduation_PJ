#include "pch.h"
#include "Player.h"
#include "ObjManager.h"
ObjManager::ObjManager() 
{

};
ObjManager::~ObjManager() {};
void ObjManager::ClientInit(int UserN)
{
	g_clients[UserN] = new Player;
	
	//플레이어 할당해놓는거 다시 생각해보자
	//임시로 지금 이렇게 해놨지만
	//정말 잘못된거임...
};
void ObjManager::SendPacket(int id, void *packet)
{
	stOverEx *ex = new stOverEx;
	memcpy(ex->m_IOCPbuf, packet, reinterpret_cast<unsigned char *>(packet)[0]);
	ex->m_todo = OP_SEND;
	ex->m_wsaBuf.buf = (char *)ex->m_IOCPbuf;
	ex->m_wsaBuf.len = ex->m_IOCPbuf[0];
	ZeroMemory(&ex->m_wsaOver, sizeof(WSAOVERLAPPED));
	printf(" 보낸다");
	int ret = WSASend(g_clients[id]->m_socket, &ex->m_wsaBuf, 1, NULL, 0, &ex->m_wsaOver, 0);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			printf("오류");
	}
}
void ObjManager::ProcessPacket(int id, unsigned char *packet)
{
	int x = g_clients[id]->m_x;
	int y = g_clients[id]->m_y;

	switch (packet[1]) { //type
	case CS_MOVE:
	{
		//이동시
		switch (packet[2]) //DIR
		{
		case UP:
			break;
		case DOWN:

			break;
		case LEFT:

			break;
		case RIGHT:
			break;
		}
		g_clients[id]->m_x = x;
		g_clients[id]->m_y = y;
	}
	//보낼 패킷에 넣어줌
	sc_packet_pos pos_packet;
	pos_packet.ID = id;
	pos_packet.size = sizeof(sc_packet_pos);
	pos_packet.type = SC_POSITION_INFO;
	pos_packet.X_POS = x;
	pos_packet.Y_POS = y;
	SendPacket(id, &pos_packet);
	printf(" 다보냄");
	}
}