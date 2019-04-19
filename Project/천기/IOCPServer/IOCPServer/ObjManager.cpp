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
		//�̵���
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
			std::wcout << L"���ǵ��� ���� ��Ŷ ����!!\n";
			while (true);
		}
		g_clients[id]->m_x = x;
		g_clients[id]->m_y = y;

		//PACKETMANAGER->PosPacket(id, x, y);
		printf("�̵� ");
	}
	case SC_MATCHING_PLAYER:
	{
		sc_packet_matching *match = reinterpret_cast<sc_packet_matching *>(packet);
		g_clients[id]->mod = match->mod;
		g_clients[id]->ready = match->ready;
		g_clients[id]->avatar = match->avatar;
		g_clients[id]->map = match->map;
		//���ӿ� ���� ���� false�� �ٲ������! ��!
		
		//��Ī�ý���
		for (int i = 0; i <= MAX_USER; ++i) {
			if(true == GetPlayer(i)->ready)
			switch (g_clients[id]->mod)
			{
			case SOLO:
			{
				//�ΰ��ӵ��ִ� �ѵ� ���ϳ��� ���������ҵ�?
				//�ΰ������� �� ü���� ���� �ϰ�
				//���� false�� �ٲ���
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
	case SC_SCENE: //�������� MATCHING�ϸ� ���Ѱ��ٲ�
	{
		switch (packet[2]) //DIR
		{
		case INGAME: {
			printf("�ΰ���\n");
			break;
		}
		default:
			std::wcout << L"���ǵ��� ���� ��Ŷ ����!!\n";
			while (true);
		}
		
	}

	}
};