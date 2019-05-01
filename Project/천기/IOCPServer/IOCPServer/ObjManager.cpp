#include "pch.h"
#include "Player.h"
#include "ObjManager.h"
#include "PacketManager.h"
#include "RoomManager.h"
#include "Functor.h"
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

void ObjManager::MatchProcess(int id, unsigned char *packet) 
{
	std::cout << "매칭요청 : " << id << std::endl;
	if (packet[1] == SC_MATCHING_PLAYER) 
	{ 
		sc_packet_matching *match = reinterpret_cast<sc_packet_matching *>(packet);
		g_clients[id]->mod = match->mod;
		//g_clients[id]->ready = match->ready;
		g_clients[id]->avatar = match->avatar;
		g_clients[id]->map = match->map;
	}
	switch (g_clients[id]->map)
	{
	case PLAYGROUND:
	{
		ModMatch(id);
		break;
	}
	case CONCERT:
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
	case SC_MOVE_PLAYER:
	{
		MovePkt(id, packet);
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
		break;
	}
	default:
		std::wcout << L"정의되지 않은 패킷 도착!!\n";
		break;
	}
	std::cout << "캐릭터 : " << g_clients[id]->avatar << std::endl;
	std::cout << "맵 : " << g_clients[id]->map << std::endl;
	std::cout << "모드 : " << g_clients[id]->mod << std::endl;
	std::cout << "아이디 : " << id << std::endl;
}
void ObjManager::MovePkt(int id, unsigned char *packet)
{
	//if ((g_clients[id]->state == PlayerState::IDLE ||
	//	g_clients[id]->state == PlayerState::RUN ||
	//	g_clients[id]->state == PlayerState::JUMPROLL ||
	//	g_clients[id]->state == PlayerState::HIT) == false)
	//	return;

	int x = g_clients[id]->m_x;
	int y = g_clients[id]->m_y;

	switch (packet[2]) //DIR
	{
	case UP:
		if (y > 0) y--;
		break;
	case DOWN:
		if (y < WORLD_HEIGHT - 1) y++;
		break;
	case LEFT:
		if (x > 0) x--;
		break;
	case RIGHT:
		if (x < WORLD_WIDTH - 1) x++;
		break;
	default:
		std::wcout << L"정의되지 않은 패킷 도착!!\n";
		while (true);
	}
	g_clients[id]->m_x = x;
	g_clients[id]->m_y = y;



	float fDistance = 0;

	 //XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
	 //if (DIR_FORWARD & packet[2]) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
	 //if (DIR_BACKWARD & packet[2]) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
	 //if (DIR_RIGHT & packet[2]) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
	 //if (DIR_LEFT & packet[2]) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
	 //if (dwDirection & packet[2]) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
	 //if (dwDirection & packet[2]) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);
	
	//DIR 클라 ojb.h 에있음 프로토콜로 옮기자
		

	PACKETMANAGER->PosPacket(id, x, y);
	printf("이동 ");
}