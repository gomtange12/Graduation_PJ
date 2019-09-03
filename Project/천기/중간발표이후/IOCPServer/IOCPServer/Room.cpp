#include "pch.h"
#include "Room.h"


Room::Room()
{
	m_flag = false;
	clocking = false;
	m_full = false;
	for (int i = 0; i < SOLO_RNUM; ++i) {
		m_SoloIds[i] = -1;
	}
	for (int i = 0; i < TEAM_RNUM; ++i) {
		m_TeamIds[i] = -1;
	}
	mod = -1;
}
Room::~Room()
{
}