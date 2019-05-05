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
			printf("����");
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
//void PacketManager::PutPlayerPacket(int id) //�ʿ������
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
			//��Ī�� 
			// �����濡 �ִ� '���' id�� ���� ���� ����� ������ ���� �ش�.
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
void PacketManager::ScenePacket(int id, int roomNum) { //Solo ��Ī���� 2�ο�
	//�ش� ä�� ��Ī�� Ŭ�� ��ο���
	sc_packet_scene packet;
	packet.size = sizeof(sc_packet_scene);
	packet.type = SC_SCENE;
	packet.sceneNum = INGAME;
	packet.roomNum = roomNum;
	for (int i = 0; i < PERSONNEL; ++i) {
		if (ROOMMANAGER->room[roomNum]->m_ids[i] != id) {
			packet.ids = ROOMMANAGER->room[roomNum]->m_ids[i];
			break;
		}
	}
	SendPacket(id, &packet);
}
void PacketManager::VectorPacket(int id)
{
	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	for (int i = 0; i < PERSONNEL; ++i) {
		sc_packet_vector packet;
		packet.size = sizeof(sc_packet_vector);
		packet.type = SC_VECTOR_INFO;
		packet.id = id;
		packet.RposX = objectManager->GetPlayer(id)->m_xmf4x4ToParent._11;
		packet.RposY = objectManager->GetPlayer(id)->m_xmf4x4ToParent._12;
		packet.RposZ = objectManager->GetPlayer(id)->m_xmf4x4ToParent._13;
		packet.LposX = objectManager->GetPlayer(id)->m_xmf4x4ToParent._31;
		packet.LposY = objectManager->GetPlayer(id)->m_xmf4x4ToParent._32;
		packet.LposZ = objectManager->GetPlayer(id)->m_xmf4x4ToParent._33;
		
		SendPacket(ROOMMANAGER->room[roomNum]->m_ids[i], &packet);
	
	}
}