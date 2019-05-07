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
	//void PutPlayerPacket(int id);
	void MovePacket(int id, const XMFLOAT3& shift);
	void ClientDisconnect(int id);
	void ScenePacket(int num, int roomNum);
	void VectorPacket(int id);
	void CollisionPacket(int id);
};
