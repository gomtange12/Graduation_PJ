#include "pch.h"
#include "Room.h"


Room::Room()
{
	m_full = false;
	for (int i = 0; i < PERSONNEL; ++i) {
		m_ids[i] = -1;
	}
}
Room::~Room()
{
}