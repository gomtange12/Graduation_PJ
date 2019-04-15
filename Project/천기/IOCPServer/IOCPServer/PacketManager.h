#pragma once
#include "ObjManager.h"

class Player;
class Packetmanager : public SingleTone<Packetmanager>
{
private:
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();
public:
	Packetmanager();
	~Packetmanager();

	void SendPacket(int id, void *packet);
	void LoginPacket(int id);
	void PutPlayerPacket(int id);
	void PosPacket(int id, int x, int y);
	void ClientDisconnect(int id);
};
