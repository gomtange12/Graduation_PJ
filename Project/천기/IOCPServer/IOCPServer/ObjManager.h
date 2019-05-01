#pragma once
#include "Protocol.h"


class Player;
class ObjManager : public SingleTone<ObjManager> 
{
private:
	std::array<Player*, MAX_USER>		g_clients;
	//std::vector<SoloRoom*> v_soloRoom;
	
	int soloRoomNum;
public:
	

	ObjManager();
	virtual ~ObjManager();
public:
	
	void ClientInit();
	void MatchProcess(int id, unsigned char *packet);
	void ProcessPacket(int id, unsigned char *packet);
	void ModMatch(int id);
	void MovePkt(int id, unsigned char *packet);
	ObjManager*			GetObjectManager() { return this; }
	inline Player*&		GetPlayer(unsigned int playerIndex) { return g_clients[playerIndex]; }
};
