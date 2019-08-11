#include "pch.h"
#include "Player.h"
#include "ObjManager.h"
#include "PacketManager.h"
#include "RoomManager.h"
#include "Functor.h"
#include "ThreadManager.h"

ObjManager::ObjManager() 
{
};
ObjManager::~ObjManager() {};
void ObjManager::ClientInit()
{
	for (Player*& p : g_clients)
	{
		p = new Player;
		p->Initialize();
	}
	soloRoomNum = 0;
};
void ObjManager::OverlappedRecv(unsigned int id)
{
	DWORD flags = 0;
	ZeroMemory(&g_clients[id]->m_RecvOverEx.m_wsaOver, sizeof(WSAOVERLAPPED));
	WSARecv(g_clients[id]->m_socket, &g_clients[id]->m_RecvOverEx.m_wsaBuf,
		1, NULL, &flags, &(g_clients[id]->m_RecvOverEx.m_wsaOver), 0);
}
void ObjManager::MatchProcess(int id, unsigned char *packet) 
{
	if (packet[1] == CS_MATCHING_PLAYER)
	{
		std::cout << "Machig Request --- " << std::endl;

		cs_packet_matching *match = reinterpret_cast<cs_packet_matching *>(packet);
		g_clients[id]->avatar = match->avatar;
		g_clients[id]->map = match->map;
		g_clients[id]->mod = match->mod;

		std::cout << "ID : " << id << std::endl;
		std::cout << "Avatar : " << g_clients[id]->avatar << std::endl;
		std::cout << "Map : " << g_clients[id]->map << std::endl;
		std::cout << "Mod : " << g_clients[id]->mod << std::endl;

		switch (g_clients[id]->map)
		{
		case PLAYGROUNDMAP:
		{
			ModMatch(id);
			break;
		}
		case CONCERTMAP:
		{
			ModMatch(id);
			break;
		}
		default:
			std::wcout << "잘못된 매칭 정보입니다\n";
			break;
		}
	}
}

void ObjManager::ProcessPacket(int id, unsigned char *packet)
{
	switch (packet[1]) //type
	{ 
	case CS_MOVE_STATE_INFO:
	{
		MovePkt(id, packet);
		break;
	}
	case CS_ROTE_STATE_INFO: 
	{
		RotePkt(id, packet);
		break;
	}

	case CS_KEY_INFO:
	{
		KeyPkt(id, packet);
		break;
	}
	case CS_LOBBY_OUT:
	{
		//LobbyPkt(id, packet);
		break;
	}
	case CS_TIME_OUT:
	{
		TimeOut(id);
		break;
	}
	default:
		break;
	}
};
void ObjManager::ModMatch(int id)
{
	switch (g_clients[id]->mod)
	{
	case SOLO:
	{
		ROOMMANAGER->SoloRoomMatch(id);
		break;
	}
	case DUO:
	{
		//ROOMMANAGER->TeamRoomMatch(id);
		break;
	}
	case SQUAD:
	{
		ROOMMANAGER->TeamRoomMatch(id);
		break;
	}
	default:
		std::wcout << L"정의되지 않은 패킷 도착!!\n";
		break;
	}
	
	
}
void ObjManager::MovePkt(int id, unsigned char *packet)
{
	cs_packet_move_state *pkt = reinterpret_cast<cs_packet_move_state *>(packet);
	
	Player* player = g_clients[id];
	XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
	float fDistance = 12.25f;

	if (DIR_FORWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Look, fDistance);}
	if (DIR_BACKWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Look, -fDistance);}
	if (DIR_LEFT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Right, -fDistance);}
	if (DIR_RIGHT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Right, fDistance);}

	player->move(xmf3Shift, true);
	g_clients[id]->m_xmOOBB.Center = XMFLOAT3(g_clients[id]->m_xmf3Position.x, g_clients[id]->m_xmf3Position.y, g_clients[id]->m_xmf3Position.z);
	bool coolCheck = collisionPlayerByPlayer(id);

	if (coolCheck == true) {
		xmf3Shift = Vector3::Add(XMFLOAT3(0, 0, 0), g_clients[id]->m_xmf3Look, -fDistance);
		player->move(xmf3Shift, true);
		g_clients[id]->m_xmOOBB.Center = XMFLOAT3(g_clients[id]->m_xmf3Position.x, g_clients[id]->m_xmf3Position.y, g_clients[id]->m_xmf3Position.z);
	}
	PACKETMANAGER->MovePacket(id);
	
	//dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->AddTimer(id, OP_MOVE, GetTickCount()+100);	
}
void ObjManager::RotePkt(int id, unsigned char *packet)
{
	cs_packet_rote_state *pkt = reinterpret_cast<cs_packet_rote_state *>(packet);

	XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&g_clients[id]->m_xmf3Up), XMConvertToRadians(pkt->y));
	g_clients[id]->m_xmf3Look = Vector3::TransformNormal(g_clients[id]->m_xmf3Look, xmmtxRotate);
	g_clients[id]->m_xmf3Right = Vector3::TransformNormal(g_clients[id]->m_xmf3Right, xmmtxRotate);

	g_clients[id]->m_xmf3Look = Vector3::Normalize(g_clients[id]->m_xmf3Look);
	g_clients[id]->m_xmf3Right = Vector3::CrossProduct(g_clients[id]->m_xmf3Up, g_clients[id]->m_xmf3Look, true);
	
	
	g_clients[id]->m_xmf4x4ToParent._11 = g_clients[id]->m_xmf3Right.x; g_clients[id]->m_xmf4x4ToParent._12 = g_clients[id]->m_xmf3Right.y; g_clients[id]->m_xmf4x4ToParent._13 = g_clients[id]->m_xmf3Right.z;
	g_clients[id]->m_xmf4x4ToParent._31 = g_clients[id]->m_xmf3Look.x; g_clients[id]->m_xmf4x4ToParent._32 = g_clients[id]->m_xmf3Look.y; g_clients[id]->m_xmf4x4ToParent._33 = g_clients[id]->m_xmf3Look.z;
	
	PACKETMANAGER->VectorPacket(id);

}
bool ObjManager::collisionPlayerByPlayer(int id)
{
	
	int roomNum = g_clients[id]->roomNumber;
	int otherId=-1;
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			if (id != ROOMMANAGER->room[roomNum]->m_SoloIds[i]) {
				otherId = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
				if (g_clients[id]->m_xmOOBB.Contains(g_clients[otherId]->m_xmOOBB)) //충돌!
				{
					//PACKETMANAGER->CollisionPacket(id, otherId);
					return true;
				}
			}
		}

	}
	else {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			if (id != ROOMMANAGER->room[roomNum]->m_TeamIds[i]) {
				otherId = ROOMMANAGER->room[roomNum]->m_TeamIds[i];
				if (g_clients[id]->m_xmOOBB.Contains(g_clients[otherId]->m_xmOOBB)) //충돌!
				{
					//PACKETMANAGER->CollisionPacket(id, otherId);
					return true;
				}
			}
		}

	}
	return false;		
}
void ObjManager::KeyPkt(int id, unsigned char *packet)
{
	cs_packet_key *pkt = reinterpret_cast<cs_packet_key *>(packet);

	PACKETMANAGER->KeyPacket(id, pkt->jump, pkt->attack , pkt->skill);
	float damageLength = 0.0f;
	int damage = 0;
	if (pkt->skill == true || pkt->attack == true)
	{
		if (pkt->attack == true) {
			damageLength = 140.0f;
			damage = 1;
		}
		else {
			damageLength = 140.0f;
			damage = 2;
		}
		int roomNum = g_clients[id]->roomNumber;
		int otherId;

		if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
			for (int i = 0; i < SOLO_RNUM; ++i) {
				if (id != ROOMMANAGER->room[roomNum]->m_SoloIds[i])
					otherId = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
			}
			float fLength = sqrtf(pow(g_clients[id]->m_xmf3Position.x - g_clients[otherId]->m_xmf3Position.x, 2) + pow(g_clients[id]->m_xmf3Position.z - g_clients[otherId]->m_xmf3Position.z, 2));
			if (fLength <= damageLength) {
				XMFLOAT3 xmf3Shift = Vector3::Add(XMFLOAT3(0, 0, 0), g_clients[id]->m_xmf3Look, 40.f);
				g_clients[otherId]->move(xmf3Shift, true);
				g_clients[otherId]->m_xmOOBB.Center = XMFLOAT3(g_clients[otherId]->m_xmf3Position.x, g_clients[otherId]->m_xmf3Position.y, g_clients[otherId]->m_xmf3Position.z);

				PACKETMANAGER->MovePacket(otherId);
				g_clients[otherId]->hp = g_clients[otherId]->hp - damage;
				PACKETMANAGER->AttackPacKet(otherId);
				if (g_clients[otherId]->hp == 0) {
					g_clients[otherId]->lose = true;
					PACKETMANAGER->DeathPacket(otherId);
					PACKETMANAGER->ResultPacket(otherId);
					dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->AddTimer(otherId, OP_LOBBY, roomNum, GetTickCount() + 4000);
				}
			}
		}
		else {
			for (int i = 0; i < TEAM_RNUM; ++i) {
				if (id != ROOMMANAGER->room[roomNum]->m_TeamIds[i]) {
					otherId = ROOMMANAGER->room[roomNum]->m_TeamIds[i];

					float fLength = sqrtf(pow(g_clients[id]->m_xmf3Position.x - g_clients[otherId]->m_xmf3Position.x, 2) + pow(g_clients[id]->m_xmf3Position.z - g_clients[otherId]->m_xmf3Position.z, 2));
					if (fLength <= damageLength) {
						XMFLOAT3 xmf3Shift = Vector3::Add(XMFLOAT3(0, 0, 0), g_clients[id]->m_xmf3Look, 40.f);
						g_clients[otherId]->move(xmf3Shift, true);
						g_clients[otherId]->m_xmOOBB.Center = XMFLOAT3(g_clients[otherId]->m_xmf3Position.x, g_clients[otherId]->m_xmf3Position.y, g_clients[otherId]->m_xmf3Position.z);

						PACKETMANAGER->MovePacket(otherId);
						g_clients[otherId]->hp = g_clients[otherId]->hp - damage;
						std::cout << g_clients[otherId]->hp << std::endl;
						PACKETMANAGER->AttackPacKet(otherId);
						if (g_clients[otherId]->hp == 0) {
							g_clients[otherId]->death = true;
						
							PACKETMANAGER->DeathPacket(otherId);
						}	
					}
				}
			}

			if (g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[0]]->death == true && g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[1]]->death == true) {
				g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[0]]->lose = true; g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[1]]->lose = true;
				PACKETMANAGER->ResultPacket(id);

				dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->AddTimer(ROOMMANAGER->room[roomNum]->m_TeamIds[0], OP_LOBBY, roomNum, GetTickCount() + 4000);
			}
			if (g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[2]]->death == true && g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[3]]->death == true) {
				g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[2]]->lose = true; g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[3]]->lose = true;
				PACKETMANAGER->ResultPacket(id);

				dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->AddTimer(ROOMMANAGER->room[roomNum]->m_TeamIds[2], OP_LOBBY, roomNum, GetTickCount() + 4000);
			}
		}
	}
}
void ObjManager::LobbyPkt(int id)
{
	int ids;
	int modNum;
	int roomNum = g_clients[id]->roomNumber;
	
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			ids = ROOMMANAGER->room[roomNum]->m_SoloIds[i];

			PACKETMANAGER->LobbyPacket(ids);
			//초기화
			if (i == 1) {
				for (int i = 0; i < SOLO_RNUM; ++i) {
					ids = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
					g_clients[ids]->death = false;
					g_clients[ids]->lose = false;
					g_clients[ids]->m_match = false;
					g_clients[ids]->hp = 8;
				}
			}
		}
	}
	else {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			ids = ROOMMANAGER->room[roomNum]->m_TeamIds[i];

			PACKETMANAGER->LobbyPacket(ids);
			//초기화
			if (i == 3) {
				for (int i = 0; i < TEAM_RNUM; ++i) {
					ids = ROOMMANAGER->room[roomNum]->m_TeamIds[i];
					g_clients[ids]->death = false;
					g_clients[ids]->lose = false;
					g_clients[ids]->m_match = false;
					g_clients[ids]->hp = 8;
				}
			}
		}
	}
	ROOMMANAGER->room[roomNum]->clocking = false;
}
void ObjManager::TimeOut(int id) {
	int roomNum = g_clients[id]->roomNumber;
	if (ROOMMANAGER->room[roomNum]->mod == SOLO) {
		for (int i = 0; i < SOLO_RNUM; ++i) {
			g_clients[ROOMMANAGER->room[roomNum]->m_SoloIds[i]]->lose = true;
		}
	}
	else if (ROOMMANAGER->room[roomNum]->mod = SQUAD) {
		for (int i = 0; i < TEAM_RNUM; ++i) {
			g_clients[ROOMMANAGER->room[roomNum]->m_TeamIds[i]]->lose = true;
		}
	}
	PACKETMANAGER->ResultPacket(id);
	dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->AddTimer(id, OP_LOBBY, roomNum, GetTickCount() + 4000);
}