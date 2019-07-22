#include "pch.h"
#include "Protocol.h"
#include "PacketManager.h"
#include "Player.h"
#include "RoomManager.h"
#include "ThreadManager.h"
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
		if (WSA_IO_PENDING != err_no);
			//printf("오류");
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


void PacketManager::ClientDisconnect(int id)
{
	//int otherId;
	//int roomNum = objectManager->GetPlayer(id)->roomNumber;
	//for (int i = 0; i < SOLO_RNUM; ++i) {
	//	if (id != ROOMMANAGER->room[roomNum]->m_SoloIds[i])
	//		otherId = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
	//}
	//
	//sc_packet_remove_player pkt;
	//pkt.id = id;
	//pkt.size = sizeof(sc_packet_remove_player);
	//pkt.type = SC_REMOVE_PLAYER;

	//LobbyPacket(id);
	//objectManager->GetPlayer(id)->gameEnd = false;
	//objectManager->GetPlayer(otherId)->gameEnd = false;
	//objectManager->GetPlayer(id)->m_match = false;
	//objectManager->GetPlayer(otherId)->m_match = false;
	//objectManager->GetPlayer(id)->hp = 6;
	//objectManager->GetPlayer(otherId)->hp = 6;
	//ROOMMANAGER->room[roomNum]->m_full = false;
	//dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->PopTimer(roomNum);

	//SendPacket(otherId, &pkt);

	closesocket(objectManager->GetPlayer(id)->m_socket);
	objectManager->GetPlayer(id)->m_connected = false;	
}
void PacketManager::IngamePacket(int id, int roomNum) {
	//해당 채널 매칭된 클라 모두에게

	sc_packet_scene pkt;
	pkt.size = sizeof(sc_packet_scene);
	pkt.type = SC_SCENE;
	pkt.sceneNum = objectManager->GetPlayer(pkt.ids[id])->map;
	//pkt.roomNum = roomNum;
	pkt.mod = ROOMMANAGER->room[roomNum]->mod;

	
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) 
		{
			pkt.ids[i] = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
			pkt.posN[i] = objectManager->GetPlayer(pkt.ids[i])->posN;
			pkt.avatar[i] = objectManager->GetPlayer(pkt.ids[i])->avatar;
		}
		SendPacket(id, &pkt);
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i)
		{
			pkt.ids[i] = ROOMMANAGER->room[roomNum]->m_TeamIds[i];
			pkt.posN[i] = objectManager->GetPlayer(pkt.ids[i])->posN;
			pkt.avatar[i] = objectManager->GetPlayer(pkt.ids[i])->avatar;
		}	
		SendPacket(id, &pkt);	
	}
	
}
void PacketManager::MovePacket(int id)
{
	//매칭시 
	// 같은방에 있는 '모든' id들 에게 나의 변경된 포지션 값을 준다.
	sc_packet_move pkt;
	pkt.size = sizeof(sc_packet_move);
	pkt.type = SC_MOVE_PLAYER;
	pkt.id = id;
	pkt.posX = objectManager->GetPlayer(id)->m_xmf3Position.x;
	pkt.posY = objectManager->GetPlayer(id)->m_xmf3Position.y;
	pkt.posZ = objectManager->GetPlayer(id)->m_xmf3Position.z;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);
		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);
		}
	}
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
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);

		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);

		}
	}
		
}
void PacketManager::CollisionPacket(int id, int otherId) {
	sc_packet_collision pkt;
	pkt.size = sizeof(sc_packet_collision);
	pkt.type = SC_COLLISION;
	pkt.id = id;
	pkt.otherid = otherId;
	pkt.check = true;
	
	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);

		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);

		}
	}
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
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);

		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);

		}
	}
}
void PacketManager::AttackPacKet(int id) 
{
	sc_packet_attack pkt;
	pkt.size = sizeof(sc_packet_attack);
	pkt.type = SC_ATTACK_INFO;
	pkt.id = id;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);

		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);

		}
	}
}
void PacketManager::LobbyPacket(int id) 
{
	sc_packet_lobby pkt;
	pkt.size = sizeof(sc_packet_lobby);
	pkt.type = SC_LOBBY_IN;


	SendPacket(id, &pkt);

	int roomNum = objectManager->GetPlayer(id)->roomNumber;

	
	//방 초기화
	ROOMMANAGER->room[roomNum]->m_full == false;
	
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			ROOMMANAGER->room[roomNum]->m_SoloIds[i] = -1;
		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			ROOMMANAGER->room[roomNum]->m_TeamIds[i] = -1;
		}
	}
}
//void PacketManager::WinPacket(int id)
//{
//	sc_packet_result pkt;
//	pkt.size = sizeof(sc_packet_result);
//	pkt.type = SC_RESULT_INFO;
//	pkt.id = id; //lose id 확인용
//	pkt.result = objectManager->GetPlayer(id)->lose;
//
//	int roomNum = objectManager->GetPlayer(id)->roomNumber;
//	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
//		for (int i = 0; i < SOLO_RNUM; ++i) {
//			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);
//
//		}
//	}
//	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
//		for (int i = 0; i < TEAM_RNUM; ++i) {
//			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);
//
//		}
//	}
//}
//void PacketManager::LosePacket(int id)
//{
//	sc_packet_result pkt;
//	pkt.size = sizeof(sc_packet_result);
//	pkt.type = SC_RESULT_INFO;
//	pkt.id = id; //lose id 확인용
//	pkt.result = objectManager->GetPlayer(id)->lose;
//
//	int roomNum = objectManager->GetPlayer(id)->roomNumber;
//	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
//		for (int i = 0; i < SOLO_RNUM; ++i) {
//			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);
//
//		}
//	}
//	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
//		for (int i = 0; i < TEAM_RNUM; ++i) {
//			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);
//
//		}
//	}
//}
void PacketManager::DeathPacket(int id) {
	sc_packet_death pkt;
	pkt.size = sizeof(sc_packet_death);
	pkt.type = SC_DEATH;
	pkt.id = id;

	int roomNum = objectManager->GetPlayer(id)->roomNumber;
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_SoloIds[i], &pkt);

		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			SendPacket(ROOMMANAGER->room[roomNum]->m_TeamIds[i], &pkt);

		}
	}
}
void PacketManager::TwitchChat(std::string &name) {
	sc_packet_chat pkt;
	pkt.type = SC_CHAT;
	strcpy(pkt.name, name.c_str());
	pkt.c_size = name.size();
	pkt.size = sizeof(sc_packet_chat);
	//pkt.chat = message;
	std::cout << sizeof(sc_packet_chat);

	for (int i = 0; i < MAX_USER; ++i) {
		if (true == objectManager->GetPlayer(i)->m_connected) {

			SendPacket(objectManager->GetPlayer(i)->m_id, &pkt);
		}
	}
}