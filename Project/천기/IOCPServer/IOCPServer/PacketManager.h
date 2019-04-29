#pragma once
#include "ObjManager.h"

class Player;
class PacketManager : public SingleTone<PacketManager>
{
private:
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();
public:
	PacketManager();
	~PacketManager();

	void SendPacket(int id, void *packet);
	void LoginPacket(int id);
	void PutPlayerPacket(int id);
	void PosPacket(int id, int x, int y);
	void ClientDisconnect(int id);
	void ScenePacket(int num, int roomNum);
};
