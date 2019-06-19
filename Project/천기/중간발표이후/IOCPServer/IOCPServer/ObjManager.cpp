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
	//std::cout << "Machig Request --- " << std::endl;

		cs_packet_matching *match = reinterpret_cast<cs_packet_matching *>(packet);
		g_clients[id]->mod = match->mod;
		//g_clients[id]->ready = match->ready;
		g_clients[id]->map = match->map;

	}
	switch (g_clients[id]->map)
	{
	case PLAYGROUNDMAP:
	{
		ModMatch(id);
		break;
	}
	case CONCERTMAP:
	{
		//ModMatch(id);
		break;
	}
	default:
		std::wcout << "잘못된 매칭 정보입니다\n";
		break;
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
		LobbyPkt(id, packet);
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
	std::cout << "ID : " << id << std::endl;
	std::cout << "Avatar : " << g_clients[id]->avatar << std::endl;
	std::cout << "Map : " << g_clients[id]->map << std::endl;
	std::cout << "Mod : " << g_clients[id]->mod << std::endl;
	
}
void ObjManager::MovePkt(int id, unsigned char *packet)
{
	cs_packet_move_state *pkt = reinterpret_cast<cs_packet_move_state *>(packet);
	
	Player* player = g_clients[id];
	XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
	float fDistance = 5.25f;

	if (DIR_FORWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Look, fDistance);}
	if (DIR_BACKWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Look, -fDistance);}
	if (DIR_LEFT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Right, -fDistance);}
	if (DIR_RIGHT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Right, fDistance);}

	player->move(xmf3Shift, true);
	
	bool coolCheck = collisionPlayerByPlayer(id);
	std::cout << g_clients[id]->m_xmf3Look.x << " " << g_clients[id]->m_xmf3Look.z << std::endl;
	std::cout << player->m_xmf3Position.x << " " << player->m_xmf3Position.z << std::endl;

	
	if (coolCheck == true) {
		g_clients[id]->m_xmf3Position = Vector3::Add(g_clients[id]->m_xmf3Position, g_clients[id]->m_xmf3Look, -fCollDistance);
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
	int otherId;
	for (int i = 0; i < SOLO_RNUM; ++i) {
		if (id != ROOMMANAGER->room[roomNum]->m_SoloIds[i])
			otherId = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
	}
			
	if (g_clients[id]->m_xmOOBB.Contains(g_clients[otherId]->m_xmOOBB)) //충돌!
	{
		PACKETMANAGER->CollisionPacket(id, otherId);
		g_clients[otherId]->m_xmf3Position = Vector3::Add(g_clients[otherId]->m_xmf3Position, g_clients[otherId]->m_xmf3Look, -fCollDistance);
		g_clients[otherId]->m_xmOOBB.Center = XMFLOAT3(g_clients[otherId]->m_xmf3Position.x, g_clients[otherId]->m_xmf3Position.y, g_clients[otherId]->m_xmf3Position.z);
		return true;
	}
	else {
		
		return false;
	}
	
	
}
void ObjManager::KeyPkt(int id, unsigned char *packet)
{
	cs_packet_key *pkt = reinterpret_cast<cs_packet_key *>(packet);

	PACKETMANAGER->KeyPacket(id, pkt->jump, pkt->attack , pkt->skill);

	if (pkt->attack == true) 
	{
		int roomNum = g_clients[id]->roomNumber;
		int otherId;
		for (int i = 0; i < SOLO_RNUM; ++i) {
			if(id != ROOMMANAGER->room[roomNum]->m_SoloIds[i])
				otherId = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
		}
		float fLength = sqrtf(pow(g_clients[id]->m_xmf3Position.x-g_clients[otherId]->m_xmf3Position.x,2) + pow(g_clients[id]->m_xmf3Position.z-g_clients[otherId]->m_xmf3Position.z,2));
		if (fLength <= 180.0f) {
			PACKETMANAGER->AttackPacKet(otherId);
			--g_clients[otherId]->hp;
			if (g_clients[otherId]->hp <= 0) PACKETMANAGER->ResultPacket(otherId);
		}
	}
	
}
void ObjManager::LobbyPkt(int id, unsigned char *packet)
{
	cs_packet_lobby_out *pkt = reinterpret_cast<cs_packet_lobby_out *>(packet);

	g_clients[id]->gameEnd =true;

	int roomNum = g_clients[id]->roomNumber;
	int otherId;
	for (int i = 0; i < SOLO_RNUM; ++i) {
		if (id != ROOMMANAGER->room[roomNum]->m_SoloIds[i])
			otherId = ROOMMANAGER->room[roomNum]->m_SoloIds[i];
	}

	if (g_clients[id]->gameEnd == true && g_clients[otherId]->gameEnd == true) {
		
		PACKETMANAGER->LobbyPacket(id);
		//초기화
		g_clients[id]->gameEnd = false;
		g_clients[otherId]->gameEnd = false;
		g_clients[id]->m_match = false;
		g_clients[otherId]->m_match = false;
		g_clients[id]->hp = 3;
		g_clients[otherId]->hp =3;
	}

}
