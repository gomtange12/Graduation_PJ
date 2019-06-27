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
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->posN = 1;
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->NumberByPos(1);
							objectManager->GetPlayer(room[i]->m_SoloIds[0])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_SoloIds[0])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_SoloIds[1])->avatar = B;
							objectManager->GetPlayer(room[i]->m_SoloIds[1])->posN = 5;
							objectManager->GetPlayer(room[i]->m_SoloIds[1])->NumberByPos(5);
							objectManager->GetPlayer(room[i]->m_SoloIds[1])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_SoloIds[1])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));
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
	//빈방을 찾음
	for (int i = 0; i < room.size(); ++i) {
		if (room[i]->m_full == false) {
			for (int j = 0; j < TEAM_RNUM; ++j) { //아이디를 넣어줌
				if (room[i]->m_TeamIds[j] < 0) {
					if (room[i]->m_TeamIds[j] != id) {
						room[i]->m_TeamIds[j] = id;
						if (j == TEAM_RNUM - 1)//풀방이면
						{
							room[i]->m_full = true;

							std::cout << "RoomNumber : " << RNumber << " ----> Machig Success" << " ::TEAM MOD" << std::endl;
							Room* teamRooms = new Room; //풀방이니 미리 다음방 생성
							teamRooms->RoomNumber = ++RNumber;
							teamRooms->mod = SQUAD;
							room.emplace_back(teamRooms);

							//// 매칭 디폴트값
							
							objectManager->GetPlayer(room[i]->m_TeamIds[0])->avatar = A;
							objectManager->GetPlayer(room[i]->m_TeamIds[0])->posN = 1;
							objectManager->GetPlayer(room[i]->m_TeamIds[0])->NumberByPos(1);
							objectManager->GetPlayer(room[i]->m_TeamIds[0])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[0])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[0])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[0])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[1])->avatar = B;
							objectManager->GetPlayer(room[i]->m_TeamIds[1])->posN = 2;
							objectManager->GetPlayer(room[i]->m_TeamIds[1])->NumberByPos(2);
							objectManager->GetPlayer(room[i]->m_TeamIds[1])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[1])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[1])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[1])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[2])->avatar = A;
							objectManager->GetPlayer(room[i]->m_TeamIds[2])->posN = 3;
							objectManager->GetPlayer(room[i]->m_TeamIds[2])->NumberByPos(3);
							objectManager->GetPlayer(room[i]->m_TeamIds[2])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[2])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[2])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[2])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[3])->avatar = B;
							objectManager->GetPlayer(room[i]->m_TeamIds[3])->posN = 4;
							objectManager->GetPlayer(room[i]->m_TeamIds[3])->NumberByPos(4);
							objectManager->GetPlayer(room[i]->m_TeamIds[3])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[3])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[3])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[3])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[4])->avatar = B;
							objectManager->GetPlayer(room[i]->m_TeamIds[4])->posN = 5;
							objectManager->GetPlayer(room[i]->m_TeamIds[4])->NumberByPos(5);
							objectManager->GetPlayer(room[i]->m_TeamIds[4])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[4])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[4])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[4])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[5])->avatar = B;
							objectManager->GetPlayer(room[i]->m_TeamIds[5])->posN = 6;
							objectManager->GetPlayer(room[i]->m_TeamIds[5])->NumberByPos(6);
							objectManager->GetPlayer(room[i]->m_TeamIds[5])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[5])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[5])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[5])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[6])->avatar = B;
							objectManager->GetPlayer(room[i]->m_TeamIds[6])->posN = 7;
							objectManager->GetPlayer(room[i]->m_TeamIds[6])->NumberByPos(7);
							objectManager->GetPlayer(room[i]->m_TeamIds[6])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[6])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[6])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[6])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_TeamIds[7])->avatar = B;
							objectManager->GetPlayer(room[i]->m_TeamIds[7])->posN = 8;
							objectManager->GetPlayer(room[i]->m_TeamIds[7])->NumberByPos(8);
							objectManager->GetPlayer(room[i]->m_TeamIds[7])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_TeamIds[7])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_TeamIds[7])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_TeamIds[7])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							////
							
							for (int k = 0; k < TEAM_RNUM; ++k) {
								objectManager->GetPlayer(room[i]->m_TeamIds[k])->roomNumber = i;
								objectManager->GetPlayer(room[i]->m_TeamIds[k])->m_match = true;
								
								PACKETMANAGER->IngamePacket(room[i]->m_TeamIds[k], i, objectManager->GetPlayer(room[i]->m_TeamIds[k])->avatar);
								
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