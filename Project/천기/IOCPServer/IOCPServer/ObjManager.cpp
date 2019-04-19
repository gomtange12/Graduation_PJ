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

		//PACKETMANAGER->PosPacket(id, x, y);
		printf("이동 ");
	}
	case SC_MATCHING_PLAYER:
	{
		sc_packet_matching *match = reinterpret_cast<sc_packet_matching *>(packet);
		g_clients[id]->mod = match->mod;
		g_clients[id]->ready = match->ready;
		g_clients[id]->avatar = match->avatar;
		g_clients[id]->map = match->map;
		//게임에 들어가면 레디를 false로 바꿔줘야함! 꼭!
		
		//매칭시스템
		for (int i = 0; i <= MAX_USER; ++i) {
			if(true == GetPlayer(i)->ready)
			switch (g_clients[id]->mod)
			{
			case SOLO:
			{
				//인게임들어가있는 넘들 방하나씩 만들어줘야할듯?
				//인게임으로 씬 체인지 센드 하고
				//레디 false로 바꾸자
				g_clients[id]->ready = false;
				break;
			}
			case DUO:
			{

				g_clients[id]->ready = false;
				break;
			}
			case SQUAD:
			{

				g_clients[id]->ready = false;
				break;
			}
			default:
				break;
			}
		}
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