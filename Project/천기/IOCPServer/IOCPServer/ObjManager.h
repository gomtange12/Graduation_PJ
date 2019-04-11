#pragma once

class Player;
class ObjManager : public SingleTone<ObjManager> 
{
private:
	std::array<Player*, MAX_USER>		g_clients;

public:
	ObjManager();
	virtual ~ObjManager();
public:
	
	void ClientInit();
	
	void ProcessPacket(int id, unsigned char *packet);
	ObjManager*			GetObjectManager() { return this; }
	inline Player*&		GetPlayer(unsigned int playerIndex) { return g_clients[playerIndex]; }
};
