#pragma once
#include "Protocol.h"
#include "Functor.h"


class Player;
class ObjManager : public SingleTone<ObjManager>
{
private:
	std::array<Player*, MAX_USER>		g_clients;
	
	int soloRoomNum;
	float fCollDistance = 40.0f;

	std::mutex workLock;
	
public:


	ObjManager();
	virtual ~ObjManager();
public:
	inline ObjManager*			GetObjectManager() { return this; }
	inline Player*&		GetPlayer(unsigned int playerIndex) { return g_clients[playerIndex]; }

	void ClientInit();
	void OverlappedRecv(unsigned int id);
	void MatchProcess(int id, unsigned char *packet);
	void ProcessPacket(int id, unsigned char *packet);
	void ModMatch(int id);
	void MovePkt(int id, unsigned char *packet);
	void RotePkt(int id, unsigned char *packet);
	bool collisionPlayerByPlayer(int id);
	void KeyPkt(int id, unsigned char *packet);
	void LobbyPkt(int id, unsigned char *packet);
};
