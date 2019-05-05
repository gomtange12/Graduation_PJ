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
		std::wcout << "잘못된 매칭 정보입니다\n";
		break;
	}
}

void ObjManager::ProcessPacket(int id, unsigned char *packet)
{
	switch (packet[1]) //type
	{ 
	case SC_MOVE_STATE_INFO:
	{
		MovePkt(id, packet);
		break;
	}
	case SC_ROTE_STATE_INFO: {
		RotePkt(id, packet);
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
	cs_packet_move_state *pkt = reinterpret_cast<cs_packet_move_state *>(packet);
	

	XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
	float fDistance = 12.25f;

	if (DIR_FORWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Look, fDistance);
		std::wcout << L"상 ";
	}
	if (DIR_BACKWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Look, -fDistance);
		std::wcout << L"하 ";
	}
	if (DIR_LEFT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Right, -fDistance);
		std::wcout << L"좌 ";
	}
	if (DIR_RIGHT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, g_clients[id]->m_xmf3Right, fDistance);
		std::wcout << L"우 ";
	}
	
	
	PACKETMANAGER->PosPacket(id, xmf3Shift);
	
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