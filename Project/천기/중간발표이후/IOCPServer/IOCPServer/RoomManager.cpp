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
			for (int j = 0; j < SOLO_NUM; ++j) { //���̵� �־���
				if (room[i]->m_ids[j] < 0) {
					if (room[i]->m_ids[j] != id) {
						room[i]->m_ids[j] = id;
						if (j == SOLO_NUM - 1)//Ǯ���̸�
						{
							room[i]->m_full = true;
							//dynamic_cast<TimerThread*>(THREADMANAGER->FindThread(TIMER_TH))->AddTimer(id, OP_ALLPOS, RNumber, GetTickCount() + 4000);
							std::cout << "RoomNumber : " << RNumber << " ----> Machig Success" << std::endl;
							Room* soloRooms = new Room; //Ǯ���̴� �̸� ������ ����
							soloRooms->RoomNumber = ++RNumber;
							room.emplace_back(soloRooms);
							
							//// ��Ī ����Ʈ��
							objectManager->GetPlayer(room[i]->m_ids[0])->avatar = A;
							objectManager->GetPlayer(room[i]->m_ids[0])->m_xmf3Position = XMFLOAT3(-530, 10, 745);
							objectManager->GetPlayer(room[i]->m_ids[0])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_ids[0])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_ids[0])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_ids[0])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));

							objectManager->GetPlayer(room[i]->m_ids[1])->avatar = B;
							objectManager->GetPlayer(room[i]->m_ids[1])->m_xmf3Position = XMFLOAT3(1060, 10, 745);
							objectManager->GetPlayer(room[i]->m_ids[1])->SetOOBB(XMFLOAT3(objectManager->GetPlayer(room[i]->m_ids[1])->m_xmf3Position.x,
								objectManager->GetPlayer(room[i]->m_ids[1])->m_xmf3Position.y,
								objectManager->GetPlayer(room[i]->m_ids[1])->m_xmf3Position.z), XMFLOAT3(8, 10, 8), XMFLOAT4(0, 0, 0, 1));
							////
							for (int k = 0; k < SOLO_NUM; ++k) {
								objectManager->GetPlayer(room[i]->m_ids[k])->roomNumber = i;
								objectManager->GetPlayer(room[i]->m_ids[k])->m_match = true;
								
								PACKETMANAGER->IngamePacket(room[i]->m_ids[k], i, objectManager->GetPlayer(room[i]->m_ids[k])->avatar);
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