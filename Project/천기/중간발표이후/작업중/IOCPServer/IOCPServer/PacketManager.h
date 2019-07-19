#pragma once
#include "ObjManager.h"

class Player;
class PacketManager : public SingleTone<PacketManager>
{
private:
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();

	std::mutex workLock;
public:
	PacketManager();
	~PacketManager();

	void SendPacket(int id, void *packet);
	void LoginPacket(int id);
	//void PutPlayerPacket(int id);
	void ClientDisconnect(int id);
	void IngamePacket(int num, int roomNum);
	void MovePacket(int id);
	void VectorPacket(int id);
	void CollisionPacket(int id, int otherId);
	void KeyPacket(int id, bool jump, bool attack, bool skill);
	void AttackPacKet(int id);
	void LobbyPacket(int id);
	void WinPacket(int id);
	void LosePacket(int id);
	void DeathPacket(int id);
	void TwitchChat(std::string &name);
};
