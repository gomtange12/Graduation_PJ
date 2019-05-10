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
	std::cout << "Machig Request --- " << std::endl;
	if (packet[1] == CS_MATCHING_PLAYER) 
	{ 

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
	case CS_POS_INFO: 
	{
		PosPkt(id, packet);
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
	float fDistance = 12.25f;

	if (DIR_FORWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Look, fDistance);}
	if (DIR_BACKWARD & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Look, -fDistance);}
	if (DIR_LEFT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Right, -fDistance);}
	if (DIR_RIGHT & pkt->state) { xmf3Shift = Vector3::Add(xmf3Shift, player->m_xmf3Right, fDistance);}

	//player->move(xmf3Shift, true);

	PACKETMANAGER->MovePacket(id, xmf3Shift);
	
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
void ObjManager::PosPkt(int id, unsigned char *packet)
{
	cs_packet_pos *pkt = reinterpret_cast<cs_packet_pos *>(packet);
	
	g_clients[id]->m_xmOOBB.Center = XMFLOAT3(pkt->x, 0.0f, pkt->z);
	bool collision = collisionPlayerByPlayer(id);

	if (collision == true) {
		g_clients[id]->m_xmf3Position.x = pkt->x;
		//g_clients[id]->m_xmf3Position.y = pkt->y;
		g_clients[id]->m_xmf3Position.z = pkt->z;
		
		g_clients[id]->m_xmf3Position = Vector3::Add(g_clients[id]->m_xmf3Position, g_clients[id]->m_xmf3Look, -fDistance);
		g_clients[id]->m_xmOOBB.Center = XMFLOAT3(g_clients[id]->m_xmf3Position.x, g_clients[id]->m_xmf3Position.y, g_clients[id]->m_xmf3Position.z);
		//std::cout << g_clients[id]->m_xmf3Position.x << " " << g_clients[id]->m_xmf3Position.z << std::endl;
			
	}
	else {
		g_clients[id]->m_xmf3Position.x = pkt->x;
		//g_clients[id]->m_xmf3Position.y = pkt->y;
		g_clients[id]->m_xmf3Position.z = pkt->z;
		
	}
	
}
bool ObjManager::collisionPlayerByPlayer(int id)
{
	
	int roomNum = g_clients[id]->roomNumber;
	int otherId;
	for (int i = 0; i < SOLO_NUM; ++i) {
		if (id != ROOMMANAGER->room[roomNum]->m_ids[i])
			otherId = ROOMMANAGER->room[roomNum]->m_ids[i];
	}
			
	if (g_clients[id]->m_xmOOBB.Contains(g_clients[otherId]->m_xmOOBB)) //충돌!
	{
		PACKETMANAGER->CollisionPacket(id, otherId);
		g_clients[otherId]->m_xmf3Position = Vector3::Add(g_clients[otherId]->m_xmf3Position, g_clients[otherId]->m_xmf3Look, -fDistance);
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
		for (int i = 0; i < SOLO_NUM; ++i) {
			if(id != ROOMMANAGER->room[roomNum]->m_ids[i])
				otherId = ROOMMANAGER->room[roomNum]->m_ids[i];
		}
		float fLength = sqrtf(pow(g_clients[id]->m_xmf3Position.x-g_clients[otherId]->m_xmf3Position.x,2) + pow(g_clients[id]->m_xmf3Position.z-g_clients[otherId]->m_xmf3Position.z,2));
		if (fLength <= 180.0f) {
			PACKETMANAGER->AttackPacKet(otherId);
		}
	}
	
}
void ObjManager::LobbyPkt(int id, unsigned char *packet)
{
	cs_packet_lobby_out *pkt = reinterpret_cast<cs_packet_lobby_out *>(packet);

	g_clients[id]->gameEnd = pkt->out;

	int roomNum = g_clients[id]->roomNumber;
	int otherId;
	for (int i = 0; i < SOLO_NUM; ++i) {
		if (id != ROOMMANAGER->room[roomNum]->m_ids[i])
			otherId = ROOMMANAGER->room[roomNum]->m_ids[i];
	}

	if (g_clients[id]->gameEnd == pkt->out && g_clients[otherId]->gameEnd == pkt->out) {
		PACKETMANAGER->LobbyPacket(id);
		g_clients[id]->gameEnd = false;
		g_clients[otherId]->gameEnd = false;
		g_clients[id]->m_match = false;
		g_clients[otherId]->m_match = false;
	}

}
void ObjManager::MoveUpdate(int id, unsigned int time)
{
	/*timerLock.lock();
	Player* player = g_clients[id];

	player->m_xmf3Velocity = Vector3::Add(player->m_xmf3Velocity, player->m_xmf3Gravity);
	float fLength = sqrtf(player->m_xmf3Velocity.x *  player->m_xmf3Velocity.x + player->m_xmf3Velocity.z *  player->m_xmf3Velocity.z);
	
	float fMaxVelocityXZ = player->m_fMaxVelocityXZ;
	if (fLength > player->m_fMaxVelocityXZ)
	{
		player->m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		player->m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = player->m_fMaxVelocityY;
	fLength = sqrtf(player->m_xmf3Velocity.y * player->m_xmf3Velocity.y);
	if (fLength > player->m_fMaxVelocityY) player->m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(player->m_xmf3Velocity, 2, false);

	player->move(xmf3Velocity, false);

	XMFLOAT3 xmf3PlayerPosition = player->m_xmf3Position;
	float fHeight = 0;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = player->m_xmf3Velocity;
		xmf3PlayerVelocity.y = 0.0f;
		player->SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		player->m_xmf3Position = xmf3PlayerPosition;
	}
	fLength = Vector3::Length(player->m_xmf3Velocity);
	float fDeceleration = (player->m_fFriction * time);
	if (fDeceleration > fLength) fDeceleration = fLength;
	player->m_xmf3Velocity = Vector3::Add(player->m_xmf3Velocity, Vector3::ScalarProduct(player->m_xmf3Velocity, -fDeceleration, true));


	player->Transform();

	cout << player->m_xmf3Position.x << " , " << player->m_xmf3Position.y << " , " << player->m_xmf3Position.z << endl;
	
	timerLock.unlock();*/

}