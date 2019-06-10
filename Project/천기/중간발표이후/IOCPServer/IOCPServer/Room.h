#pragma once
#include "Define.h"

class Room
{
public:
	int RoomNumber;
	int m_SoloIds[SOLO_RNUM];
	int m_TeamIds[TEAM_RNUM];
	int mod;
	bool m_full;
public:
	Room();
	~Room();

private:

};
