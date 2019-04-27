#pragma once
#include <unordered_map>
#include "Room.h"




class RoomManager
{
public:
	std::unordered_map<Room> room;
public:
	RoomManager();
	~RoomManager();

	void ModMatch();
private:

};

