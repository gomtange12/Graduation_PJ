#pragma once
#include "Protocol.h"
#include "Functor.h"


class Player;
class ObjManager : public SingleTone<ObjManager>
{
private:
	std::array<Player*, MAX_USER>		g_clients;
	
	int soloRoomNum;
	float fDistance = 12.25f;

	std::mutex workLock;
public:


	ObjManager();
	virtual ~ObjManager();
public:

	void ClientInit();
	void OverlappedRecv(unsigned int id);
	void MatchProcess(int id, unsigned char *packet);
	void ProcessPacket(int id, unsigned char *packet);
	void ModMatch(int id);
	void MovePkt(int id, unsigned char *packet);
	ObjManager*			GetObjectManager() { return this; }
	inline Player*&		GetPlayer(unsigned int playerIndex) { return g_clients[playerIndex]; }
	void RotePkt(int id, unsigned char *packet);
	void PosPkt(int id, unsigned char *packet);
	bool collisionPlayerByPlayer(int id);
	void KeyPkt(int id, unsigned char *packet);
	void MoveUpdate(int player, unsigned int time);
};
