#include "pch.h"
#include "SoloRoom.h"


SoloRoom::SoloRoom()
{
	m_full = false;
	for (int i = 0; i < PERSONNEL; ++i) {
		m_ids[i] = -1;
	}
}
SoloRoom::~SoloRoom()
{
}