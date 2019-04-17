#include "pch.h"
#include "Player.h"
#include "ObjManager.h"
#include "PacketManager.h"
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

};

void ObjManager::ProcessPacket(int id, unsigned char *packet)
{
	int x = g_clients[id]->m_x;
	int y = g_clients[id]->m_y;

	switch (packet[1]) //type
	{ 
	case SC_MOVE_PLAYER:
	{
		//이동시
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

		PACKETMANAGER->PosPacket(id, x, y);
		printf("이동 ");
	}
	case SC_MATCHING_PLAYER:
	{
		//sc_packet_matching *match = reinterpret_cast<sc_packet_matching *>(packet);
		//g_clients[id]->mod = match->mod;
		//for (int i = 0; i <= MAX_USER; ++i) {
		//	if(true == GetPlayer(i)->m_connected)
		//	switch (g_clients[id]->mod)
		//	{
		//	default:
		//		break;
		//	}
		//	g_clients[id]->charac = match->charac;
		//	g_clients[id]->map = match->map;
		//	g_clients[id]->ready = match->ready;
		//	//여기에 모두 레디 되면 씬 센드 해줘야함
		//}
	}
	case SC_SCENE: //없어지고 MATCHING하면 씬넘겨줄꺼
	{
		switch (packet[2]) //DIR
		{
		case INGAME: {
			printf("인게임\n");
			break;
		}
		default:
			std::wcout << L"정의되지 않은 패킷 도착!!\n";
			while (true);
		}
		
	}

	}
};