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
	sc_packet_login_ok pkt;
	pkt.id = id;
	pkt.size = sizeof(sc_packet_login_ok);
	pkt.type = SC_LOGIN_OK;
	SendPacket(id, &pkt);
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
void PacketManager::MovePacket(int id, const XMFLOAT3& shift)
{
	//매칭시 
			// 같은방에 있는 '모든' id들 에게 나의 변경된 포지션 값을 준다.
	sc_packet_move pkt;
	pkt.size = sizeof(sc_packet_move);
	pkt.type = SC_MOVE_PLAYER;
	pkt.id = id;
	pkt.velocity = true;
	pkt.posX = shift.x;
	pkt.posY = shift.y;
	pkt.posZ = shift.z;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pkt);
	}

		
}
void PacketManager::ClientDisconnect(int id)
{
	for (int i = 0; i < MAX_USER; ++i) {
		if (false == objectManager->GetPlayer(i)->m_connected) continue;
		if (i == id) continue;
		sc_packet_remove_player pkt;
		pkt.id = id;
		pkt.size = sizeof(sc_packet_remove_player);
		pkt.type = SC_REMOVE_PLAYER;
		SendPacket(i, &pkt);

	}
	closesocket(objectManager->GetPlayer(id)->m_socket);
	objectManager->GetPlayer(id)->m_connected = false;
}
void PacketManager::IngamePacket(int id, int roomNum, int avatar) { //Solo 매칭용임 2인용
	//해당 채널 매칭된 클라 모두에게
	sc_packet_scene pkt;
	pkt.size = sizeof(sc_packet_scene);
	pkt.type = SC_SCENE;
	pkt.sceneNum = PLAYGROUNDMAP;
	pkt.roomNum = roomNum;
	pkt.avatar = avatar;
	
	if (ROOMMANAGER->room[roomNum]->m_ids[0] != id) {
		pkt.ids = ROOMMANAGER->room[roomNum]->m_ids[0];
	}
	else{ 
		pkt.ids = ROOMMANAGER->room[roomNum]->m_ids[1]; 
	}
	
	SendPacket(id, &pkt);
}
void PacketManager::VectorPacket(int id)
{
	sc_packet_vector pkt;
	pkt.size = sizeof(sc_packet_vector);
	pkt.type = SC_VECTOR_INFO;
	pkt.id = id;
	pkt.RposX = objectManager->GetPlayer(id)->m_xmf4x4ToParent._11;
	pkt.RposY = objectManager->GetPlayer(id)->m_xmf4x4ToParent._12;
	pkt.RposZ = objectManager->GetPlayer(id)->m_xmf4x4ToParent._13;
	pkt.LposX = objectManager->GetPlayer(id)->m_xmf4x4ToParent._31;
	pkt.LposY = objectManager->GetPlayer(id)->m_xmf4x4ToParent._32;
	pkt.LposZ = objectManager->GetPlayer(id)->m_xmf4x4ToParent._33;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pkt);

	}
		
}
void PacketManager::CollisionPacket(int id, int otherId) {
	sc_packet_collision pkt;
	pkt.size = sizeof(sc_packet_collision);
	pkt.type = SC_COLLISION;
	pkt.id = id;
	pkt.check = true;

	SendPacket(id, &pkt);
	SendPacket(otherId, &pkt);
}
void PacketManager::KeyPacket(int id, bool jump, bool attack, bool skill)
{
	sc_packet_key pkt;
	pkt.size = sizeof(sc_packet_key);
	pkt.type = SC_KEY_INFO;
	pkt.id = id;
	pkt.jump = jump;
	pkt.attack = attack;
	pkt.skill = skill;
	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pkt);
	}
}
void PacketManager::AttackPacKet(int id) 
{
	sc_packet_attack pkt;
	pkt.size = sizeof(sc_packet_attack);
	pkt.type = SC_ATTACK_INFO;
	pkt.id = id;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pkt);
	}
}
void PacketManager::LobbyPacket(int id) 
{
	sc_packet_lobby pkt;
	pkt.size = sizeof(sc_packet_lobby);
	pkt.type = SC_LOBBY_IN;
	pkt.out = true;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pkt);
	}

	
	//방 초기화
	ROOMMANAGER->room[roomNum]->m_full == false;
	for (int i = 0; i < SOLO_NUM; ++i) {
		ROOMMANAGER->room[roomNum]->m_ids[i] = -1;
	}
}
void PacketManager::ResultPacket(int id)
{
	sc_packet_result pkt;
	pkt.size = sizeof(sc_packet_result);
	pkt.type = SC_RESULT_INFO;
	pkt.id = id; //lose id 확인용


	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &pkt);
	}
}
void PacketManager::AllPos(int id) 
{
	int otherId;
	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < SOLO_NUM; ++i) {
		if(id!=ROOMMANAGER->room[roomNum]->m_ids[i])
			otherId = ROOMMANAGER->room[roomNum]->m_ids[i];
	}
	sc_packet_allpos pkt;
	pkt.size = sizeof(sc_packet_allpos);
	pkt.type = SC_ALL_POS;
	pkt.id = id;
	pkt.posX = objectManager->GetPlayer(otherId)->m_xmf3Position.x;
	pkt.posZ = objectManager->GetPlayer(otherId)->m_xmf3Position.z;
	SendPacket(id, &pkt);
	SendPacket(otherId, &pkt);
}