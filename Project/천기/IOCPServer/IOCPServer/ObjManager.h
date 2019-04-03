#pragma once
#include "Protocol.h"

class Player;
class ObjManager : public SingleTone<ObjManager> {
public:
	
	Player* g_clients[10];
	//std::vector<Player*> g_clients;

public:
	ObjManager();
	virtual ~ObjManager();
public:
	//Player** GetClient() { return g_clients; }
	void ClientInit(int UserN);
	void SendPacket(int id, void *packet);
	void ProcessPacket(int id, unsigned char *packet);
	
};
