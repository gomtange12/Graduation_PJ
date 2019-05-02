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
	std::cout << "��Ī��û : " << id << std::endl;
	if (packet[1] == SC_MATCHING_PLAYER) 
	{ 
		cs_packet_matching *match = reinterpret_cast<cs_packet_matching *>(packet);
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

void ObjManager::ProcessPacket(int id, unsigned char *packet)
{
	switch (packet[1]) //type
	{ 
	case SC_STATE_INFO:
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
		std::wcout << L"���ǵ��� ���� ��Ŷ ����!!\n";
		break;
	}
	std::cout << "ĳ���� : " << g_clients[id]->avatar << std::endl;
	std::cout << "�� : " << g_clients[id]->map << std::endl;
	std::cout << "��� : " << g_clients[id]->mod << std::endl;
	std::cout << "���̵� : " << id << std::endl;
}
void ObjManager::MovePkt(int id, unsigned char *packet)
{
	cs_packet_state *pkt = reinterpret_cast<cs_packet_state *>(packet);
	g_clients[id]->m_xmf3Look = XMFLOAT3(pkt->LposX, pkt->LposY, pkt->LposZ);
	g_clients[id]->m_xmf3Right = XMFLOAT3(pkt->RposX, pkt->RposY, pkt->RposZ);
	/*if ((g_clients[id]->state == PlayerState::IDLE ||
		g_clients[id]->state == PlayerState::RUN ||
		g_clients[id]->state == PlayerState::JUMPROLL ||
		g_clients[id]->state == PlayerState::HIT) == false)
		return;*/
	XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
	float fDistance = 12.25f;
	if (DIR_FORWARD & pkt->state) {
		xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Look, fDistance);
		std::wcout << L"�� ";
	}
	if (DIR_BACKWARD & pkt->state) {
		xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Look, -fDistance);
		std::wcout << L"�� ";
	}
	if (DIR_LEFT & pkt->state) {
		xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Right, -fDistance);
		std::wcout << L"�� ";
	}
	if (DIR_RIGHT & pkt->state) {
		xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Right, fDistance);
		std::wcout << L"�� ";
	}

	PACKETMANAGER->PosPacket(id, xmf3Shift);
	
}