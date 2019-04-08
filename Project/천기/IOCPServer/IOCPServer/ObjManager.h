#pragma once

class Player;
class ObjManager : public SingleTone<ObjManager> {
public:

	Player* g_clients[10];

public:
	ObjManager();
	virtual ~ObjManager();
public:
	//Player** GetClient() { return g_clients; }
	void ClientInit(int UserN);
	
	void ProcessPacket(int id, unsigned char *packet);
	
};
