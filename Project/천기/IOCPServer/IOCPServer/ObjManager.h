#pragma once

class Player;
class ObjManager : public SingleTone<ObjManager> {
public:
	std::array<Player*, MAX_USER>		g_clients;
	//Player* g_clients[MAX_USER];

public:
	ObjManager();
	virtual ~ObjManager();
public:
	
	void ClientInit();
	
	void ProcessPacket(int id, unsigned char *packet);
	ObjManager*			GetObjectManager() { return this; }
	inline Player*&		GetPlayer(unsigned int playerIndex) { return g_clients[playerIndex]; }
};
