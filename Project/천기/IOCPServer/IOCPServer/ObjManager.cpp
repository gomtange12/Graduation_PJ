#include "pch.h"
#include "Player.h"
#include "SoloRoom.h"
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
	soloRoomNum = 0;
};

void ObjManager::MatchProcess(int id, unsigned char *packet) 
{
	std::cout << "��Ī " << id << std::endl;
	if (packet[1] == SC_MATCHING_PLAYER) { //type

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
		std::wcout << "�߸��� ��Ī �����Դϴ�\n";
		break;
	}
	
}
void ObjManager::ModMatch(int id)
{
	
	switch (g_clients[id]->mod)
	{
	case SOLO:
	{
		if (v_soloRoom.size() == 0) { // �� �� ����
			SoloRoom* sr = new SoloRoom;
			v_soloRoom.emplace_back(sr);
		}
		//�ַη� �տ������� �ο��� ����ִ� �ַη��� ã��
		for (auto& vsr : v_soloRoom) {
			if (vsr->m_full == false) {
				for (int i = 0; i <= PERSONNEL; ++i) {
					if (vsr->m_ids[i] < 0) {
						vsr->m_ids[i] = id;
						g_clients[i]->m_match = true;
						if(vsr->m_ids[1] >= 0) //Ǯ��?
							vsr->m_full = true;
					}
					//
					if (vsr->m_full == true) { //Ǯ������� ������
						for (int i = 0; i <= PERSONNEL; ++i) {
							PACKETMANAGER->ScenePacket(vsr->m_ids[i]);
						}
						//Ǯ���̴� ���ο�� �ϳ� ����� �α�
						
						SoloRoom* sr = new SoloRoom;
						v_soloRoom.emplace_back(sr);
						break;
					} //Ǯ�� �ƴϸ�
					else {
						break;
					}
					
				}
			}
		}
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
		break;
	}
	std::cout << g_clients[id]->avatar << std::endl;
	std::cout << g_clients[id]->map << std::endl;
	std::cout << g_clients[id]->mod << std::endl;
	std::cout << id << std::endl;
}
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

		PACKETMANAGER->PosPacket(id, x, y);
		printf("�̵� ");
	}
	

	}
};