#pragma once
#include "Room.h"
#include "ObjManager.h"

class RoomManager : public SingleTone<RoomManager>
{
public:
	std::vector<Room*> room;
	//벡터를 사용했지만 안되는걸 알고있다.
	//unordered_map을 사용하여 만드려 했지만 해쉬함수를 가진 클레스를 만들어 사용하지 못하였다.
	//나중에 시간이 생긴다면 바꾸자.
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();

	int soloCount = 0;
	int teamCount = 0;
	int RNumber = 0;
public:
	RoomManager();
	~RoomManager();

	void SoloRoomMatch(int id);
	void TeamRoomMatch(int id);
private:

};

