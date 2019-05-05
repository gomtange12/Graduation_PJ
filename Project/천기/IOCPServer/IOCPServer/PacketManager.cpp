#include "pch.h"
#include "Protocol.h"
#include "PacketManager.h"
#include "Player.h"
#include "RoomManager.h"
PacketManager::PacketManager()
{
};

PacketManager::~PacketManager()
{
};
void PacketManager::SendPacket(int id, void *packet)
{
	stOverEx *ex = new stOverEx;
	memcpy(ex->m_IOCPbuf, packet, reinterpret_cast<unsigned char *>(packet)[0]);
	ex->m_todo = OP_SEND;
	ex->m_wsaBuf.buf = (char *)ex->m_IOCPbuf;
	ex->m_wsaBuf.len = ex->m_IOCPbuf[0];
	ZeroMemory(&ex->m_wsaOver, sizeof(WSAOVERLAPPED));

	int ret = WSASend(objectManager->GetPlayer(id)->m_socket, &ex->m_wsaBuf, 1, NULL, 0, &ex->m_wsaOver, 0);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			printf("오류");
	}
};
void PacketManager::LoginPacket(int id) 
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof(sc_packet_login_ok);
	packet.type = SC_LOGIN_OK;
	SendPacket(id, &packet);
}
//void PacketManager::PutPlayerPacket(int id) //필요없을듯
//{
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (true == objectManager->GetPlayer(i)->m_connected) {
//			sc_packet_put_player packet;
//			packet.id = id;
//		/*	packet.x = objectManager->GetPlayer(id)->m_x;
//			packet.y = objectManager->GetPlayer(id)->m_y;*/
//			packet.size = sizeof(sc_packet_put_player);
//			packet.type = SC_PUT_PLAYER;
//			SendPacket(i, &packet);
//		}
//	}
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (false == objectManager->GetPlayer(i)->m_connected) continue;
//		if (i == id) continue;
//		sc_packet_put_player packet;
//		packet.id = i;
//		/*packet.x = objectManager->GetPlayer(i)->m_x;
//		packet.y = objectManager->GetPlayer(i)->m_y;*/
//		packet.size = sizeof(sc_packet_put_player);
//		packet.type = SC_PUT_PLAYER;
//		SendPacket(id, &packet);
//	}	
//	
//};
void PacketManager::PosPacket(int id, const XMFLOAT3& shift)
{
	// 포지션값이 변경이되려면
	


	/*for (int i = 0; i < MAX_USER; ++i) {
		if (true == objectManager->GetPlayer(i)->m_connected) {*/
			//sc_packet_pos pos_packet;
			//pos_packet.size = sizeof(sc_packet_pos);
			//pos_packet.type = SC_MOVE_PLAYER;
			//pos_packet.id = id;
			//pos_packet.velocity = true;
			//pos_packet.posX = shift.x;
			//pos_packet.posY = shift.y;
			//pos_packet.posZ = shift.z;

			//SendPacket(id, &pos_packet);
	//	}
	//}

			//매칭시 
			// 같은방에 있는 '모든' id들 에게 나의 변경된 포지션 값을 준다.
			int roomNum = objectManager->GetPlayer(id)->roomNumber;
			for (int i = 0; i < PERSONNEL; ++i) {

				sc_packet_pos pos_packet;
				pos_packet.size = sizeof(sc_packet_pos);
				pos_packet.type = SC_MOVE_PLAYER;
				pos_packet.id = id;
				pos_packet.velocity = true;
				pos_packet.posX = shift.x;
				pos_packet.posY = shift.y;
				pos_packet.posZ = shift.z;
				
				SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pos_packet);
			}
}
void PacketManager::ClientDisconnect(int id)
{
	for (int i = 0; i < MAX_USER; ++i) {
		if (false == objectManager->GetPlayer(i)->m_connected) continue;
		if (i == id) continue;
		sc_packet_remove_player packet;
		packet.id = id;
		packet.size = sizeof(sc_packet_remove_player);
		packet.type = SC_REMOVE_PLAYER;
		SendPacket(i, &packet);

	}
	closesocket(objectManager->GetPlayer(id)->m_socket);
	objectManager->GetPlayer(id)->m_connected = false;
}
void PacketManager::ScenePacket(int id, int roomNum) {
	//해당 채널 매칭된 클라 모두에게
	sc_packet_scene packet;
	packet.sceneNum = INGAME;
	packet.roomNum = roomNum;
	packet.size = sizeof(sc_packet_scene);
	packet.type = SC_SCENE;
	SendPacket(id, &packet);
}
