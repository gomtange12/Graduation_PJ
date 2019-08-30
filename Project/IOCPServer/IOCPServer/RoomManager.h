#pragma once
#include "Room.h"
#include "ObjManager.h"

class RoomManager : public SingleTone<RoomManager>
{
public:
	std::vector<Room*> room;
	//���͸� ��������� �ȵǴ°� �˰��ִ�.
	//unordered_map�� ����Ͽ� ����� ������ �ؽ��Լ��� ���� Ŭ������ ����� ������� ���Ͽ���.
	//���߿� �ð��� ����ٸ� �ٲ���.
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

