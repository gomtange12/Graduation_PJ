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
	//���� ����!!!!!!!!!!!
	// ����ó�� �ϳ� �߰� �ؾ���
	// ���� ���̵� �ϰ��!  xxxx
	if (room.size() == 0) {//��ó�� �����
		Room* soloRooms = new Room;
		soloRooms->RoomNumber = RNumber;
		room.emplace_back(soloRooms);
	}

	//����� ã��
	for (int i = 0; i < room.size(); ++i) {
		if (room[i]->m_full == false) {
			for (int j = 0; j < PERSONNEL; ++j) { //���̵� �־���
				if (room[i]->m_ids[j] < 0) {
					if (room[i]->m_ids[j] != id) {
						room[i]->m_ids[j] = id;
						if (j == PERSONNEL - 1)//Ǯ���̸�
						{
							room[i]->m_full = true;
							std::cout << "RoomNumber : " << RNumber << " ----> Machig Success" << std::endl;
							Room* soloRooms = new Room; //Ǯ���̴� �̸� ������ ����
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