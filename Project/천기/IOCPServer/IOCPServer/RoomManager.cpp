#include "pch.h"
#include "RoomManager.h"
#include "PacketManager.h"
#include "Player.h"
RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}
void RoomManager::SoloRoomMatch(int id) 
{
	//버그 있음!!!!!!!!!!!
	// 예외처리 하나 추가 해야함
	// 같은 아이디 일경우!  xxxx
	if (room.size() == 0) {//맨처음 방생성
		Room* soloRooms = new Room;
		soloRooms->RoomNumber = RNumber;
		room.emplace_back(soloRooms);
	}

	//빈방을 찾음
	for (int i = 0; i < room.size(); ++i) {
		if (room[i]->m_full == false) {
			for (int j = 0; j < PERSONNEL; ++j) { //아이디를 넣어줌
				if (room[i]->m_ids[j] < 0) {
					if (room[i]->m_ids[j] != id) {
						room[i]->m_ids[j] = id;
						if (j == PERSONNEL - 1)//풀방이면
						{
							room[i]->m_full = true;
							std::cout << "RoomNumber : " << RNumber << " ----> Machig Success" << std::endl;
							Room* soloRooms = new Room; //풀방이니 미리 다음방 생성
							soloRooms->RoomNumber = ++RNumber;
							room.emplace_back(soloRooms);

							for (int k = 0; k < PERSONNEL; ++k) {
								objectManager->GetPlayer(room[i]->m_ids[k])->roomNumber = i;
								objectManager->GetPlayer(room[i]->m_ids[k])->m_match = true;
								PACKETMANAGER->ScenePacket(room[i]->m_ids[k], i);
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	
}