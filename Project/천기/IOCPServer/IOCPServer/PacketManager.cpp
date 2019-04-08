#include "pch.h"
#include "Protocol.h"
#include "PacketManager.h"
#include "ObjManager.h"
#include "Player.h"

Packetmanager::Packetmanager()
{
};

Packetmanager::~Packetmanager()
{
};
void Packetmanager::SendPacket(int id, void *packet)
{
	stOverEx *ex = new stOverEx;
	memcpy(ex->m_IOCPbuf, packet, reinterpret_cast<unsigned char *>(packet)[0]);
	ex->m_todo = OP_SEND;
	ex->m_wsaBuf.buf = (char *)ex->m_IOCPbuf;
	ex->m_wsaBuf.len = ex->m_IOCPbuf[0];
	ZeroMemory(&ex->m_wsaOver, sizeof(WSAOVERLAPPED));
	printf(" 보낸다");
	int ret = WSASend(OBJMANAGER->g_clients[id]->m_socket, &ex->m_wsaBuf, 1, NULL, 0, &ex->m_wsaOver, 0);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			printf("오류");
	}
};
void Packetmanager::LoginPacking(int id) 
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof(sc_packet_login_ok);
	packet.type = SC_LOGIN_OK;
	SendPacket(id, &packet);
}
void Packetmanager::PutPlayerPacking(int id)
{
	
	if (id >= 0) {
		for (int i = 0; i < MAX_USER; ++i) {
			if (true == OBJMANAGER->g_clients[i]->m_connected) {
				sc_packet_put_player packet;
				packet.id = id;
				packet.x = OBJMANAGER->g_clients[id]->m_x;
				packet.y = OBJMANAGER->g_clients[id]->m_y;
				packet.size = sizeof(sc_packet_put_player);
				packet.type = SC_PUT_PLAYER;
				SendPacket(i, &packet);
			}
		}
		for (int i = 0; i < MAX_USER; ++i) {
			if (false == OBJMANAGER->g_clients[i]->m_connected) continue;
			if (i == id) continue;
			sc_packet_put_player packet;
			packet.id = i;
			packet.x = OBJMANAGER->g_clients[i]->m_x;
			packet.y = OBJMANAGER->g_clients[i]->m_y;
			packet.size = sizeof(sc_packet_put_player);
			packet.type = SC_PUT_PLAYER;
			SendPacket(id, &packet);
		}
	}
};
void Packetmanager::PosPacking(int id, int x, int y) 
{
	for (int i = 0; i < MAX_USER; ++i) {
		if (true == OBJMANAGER->g_clients[i]->m_connected) {
			sc_packet_pos pos_packet;
			pos_packet.ID = id;
			pos_packet.size = sizeof(sc_packet_pos);
			pos_packet.type = SC_POSITION_INFO;
			pos_packet.X_POS = x;
			pos_packet.Y_POS = y;
			SendPacket(id, &pos_packet);
		}
}