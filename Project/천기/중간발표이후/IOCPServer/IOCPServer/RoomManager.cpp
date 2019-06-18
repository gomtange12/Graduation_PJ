#include "pch.h"
#include "RoomManager.h"
#include "PacketManager.h"
#include "Player.h"
#include "ThreadManager.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}
void RoomManager::SoloRoomMatch(int id) 
{
	if (room.size() == 0) {//맨처음 방생성
		Room* soloRooms = new Room;
		soloRooms->RoomNumber = RNumber;
		soloRooms->mod = SOLO;
		room.emplace_back(soloRooms);
	}

	//빈방을 찾음
	for (int i = 0; i < room.size(); ++i) {
		if (room[i]->m_full == false) {
			for (int j = 0; j < SOLO_RNUM; ++j) { //아이디를 넣어줌
				if (room[i]->m_SoloIds[j] < 0) {
					if (room[i]->m_SoloIds[j] != id) {
						room[i]->m_SoloIds[j] = id;
						if (j == SOLO_RNUM - 1)//풀방이면
						{
							room[i]->m_full = true;
				
							std::cout << "RoomNumber : " << RNumber << " ----> Machig Success" << " ::SOLO MOD"<< std::endl;
							Room* soloRooms = new Room; //풀방이니 미리 다음방 생성
							soloRooms->RoomNumber = ++RNumber;
							soloRooms->mod = SOLO;
							room.emplace_back(soloRooms);
							
							//// 매칭 디폴트값
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->avatar = A;
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position = XMFLOAT3(2560, 10, 1745);
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->posN = 1;

							objectManager->GetPlayer(room[i]->m_SoloIds[1])->avatar = B;
							objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position = XMFLOAT3(440.0f, 10, 1745);
							objectManager->GetPlayer(room[i]->m_SoloIds[1])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));
							objectManager->GetPlayer(room[i]->m_SoloIds[1])->posN = 2;
							////

							for (int k = 0; k < SOLO_RNUM; ++k) {
								objectManager->GetPlayer(room[i]->m_SoloIds[k])->roomNumber = i;
								objectManager->GetPlayer(room[i]->m_SoloIds[k])->m_match = true;
								
								PACKETMANAGER->IngamePacket(room[i]->m_SoloIds[k], i, objectManager->GetPlayer(room[i]->m_SoloIds[k])->avatar);
							}
							
						}
					}
					break;
				}
			}
			break;
		}
	}

	
}
void RoomManager::TeamRoomMatch(int id)
{
	if (room.size() == 0) {//맨처음 방생성
		Room* teamRooms = new Room;
		teamRooms->RoomNumber = RNumber;
		teamRooms->mod = SQUAD;
		room.emplace_back(teamRooms);
	}

	std::cout << "RoomNumber : " << RNumber << " ----> Machig Success" << " ::TEAM MOD" << std::endl;
}