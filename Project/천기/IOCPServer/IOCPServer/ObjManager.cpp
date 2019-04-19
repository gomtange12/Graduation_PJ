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

};

void ObjManager::MatchProcess(int id, unsigned char *packet) 
{
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
		ModMatch(id);
		break;
	}
	}
	
}
void ObjManager::ModMatch(int id)
{
	switch (g_clients[id]->mod)
	{
	case SOLO:
	{
		if (v_soloRoom.size() == soloRoomNum) { // 빈 방 생성
			v_soloRoom.emplace_back();
		}
		//솔로룸 앞에서부터 인원이 비어있는 솔로룸을 찾음
		for (auto& vsr : v_soloRoom) {
			if (vsr->m_full == false) {
				for (int i = 0; i <= PERSONNEL; ++i) {
					if (vsr->m_ids[i] < 0) {
						vsr->m_ids[i] = id;
						if (i == 8) {
							for (int i = 0; i <= PERSONNEL; ++i) {
								//풀방됬으니 씬센드
								g_clients[i]->m_match = true;
								vsr->m_full = true;
								++soloRoomNum; //
							}
							break;
						}
					}
				}
				break;
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
}
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