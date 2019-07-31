#pragma once
#include "Define.h"

class Room
{
public:
	int RoomNumber;
	int m_SoloIds[SOLO_RNUM];
	int m_TeamIds[TEAM_RNUM];
	int mod;
	int map;
	bool m_full;
	int clock;
public:
	Room();
	~Room();

private:

};
