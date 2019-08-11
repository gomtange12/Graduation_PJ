#include "stdafx.h"
#include "CNetWork.h"
#include "CSceneManager.h"
#include "CPlayerManager.h"
#include "CChatManager.h"

#define SPAWN 9


CNetWork::CNetWork()
{
}

CNetWork::~CNetWork()
{
}

void CNetWork::MakeServer(const HWND& hWnd)
{

	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);


	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDR);

	int Result = WSAConnect(g_mysocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);

	WSAAsyncSelect(g_mysocket, hWnd, WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = MAX_BUFFER;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = MAX_BUFFER;

}
void CNetWork::SendPacket()
{
	DWORD iobyte;
	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
		printf("Error while sending SendPacket [%d]", error_code);
	}
}
void CNetWork::ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		printf("Recv Error [%d]\n", err_code);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}
void CNetWork::ProcessPacket(unsigned char *ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case SC_LOGIN_OK:
	{
		sc_packet_login_ok *packet =
			reinterpret_cast<sc_packet_login_ok *>(ptr);
		myid = packet->id;
		//firstCheck = packet->check;
		break;
	}
	case SC_SCENE:
	{
		sc_packet_scene *paket = reinterpret_cast<sc_packet_scene *>(ptr);
		SCENEMANAGER->SetScene(static_cast<SceneState>(paket->sceneNum));

		//솔로모드면
		if (paket->mod == SOLO) {
			for (int i = 0; i < 2; ++i) {
				if (myid == paket->ids[i]) {
					PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
					PLAYER->GetPlayer()->SetClientNum(myid);
					PLAYER->GetPlayer()->NumberByPos(paket->posN[i]);
					PLAYER->GetPlayer()->SetPlayerState(IDLE);
					if(i==0)
						PLAYER->GetPlayer()->teamNum = BLUETEAM;
					else
						PLAYER->GetPlayer()->teamNum = REDTEAM;
				}

				else {
					PLAYER->GetOtherPlayerMap()[0]->SetClientNum(paket->ids[i]);
					PLAYER->GetOtherPlayerMap()[0]->NumberByPos(paket->posN[i]);
					PLAYER->GetOtherPlayerMap()[0]->SetCharacterType((E_CHARACTERTYPE)paket->avatar[i]);
					PLAYER->GetOtherPlayerMap()[0]->SetPlayerState(IDLE);
					if (i == 0)
						PLAYER->GetPlayer()->teamNum = BLUETEAM;
					else
						PLAYER->GetPlayer()->teamNum = REDTEAM;
				}
			}
		}
		else if (paket->mod = SQUAD) {
			//팀모드면
			for (int i = 0; i < 2; ++i) {
				if (myid == paket->ids[i]) {
					PLAYER->GetPlayer()->teamNum = BLUETEAM;

				}
			}
			if (PLAYER->GetPlayer()->teamNum == BLUETEAM) {
				for (int i = 0; i < 2; ++i) {
					if (myid == paket->ids[i]) {
						PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
						PLAYER->GetPlayer()->SetClientNum(myid);
						PLAYER->GetPlayer()->NumberByPos(paket->posN[i]);
						PLAYER->GetPlayer()->SetPlayerState(IDLE);
					}
				}

				for (int i = 0; i < 2; ++i) {
					if (myid != paket->ids[i]) {
						PLAYER->GetTeamPlayerMap()[i]->SetClientNum(paket->ids[i]);
						PLAYER->GetTeamPlayerMap()[i]->NumberByPos(paket->posN[i]);
						PLAYER->GetTeamPlayerMap()[i]->SetCharacterType((E_CHARACTERTYPE)paket->avatar[i]);
						PLAYER->GetTeamPlayerMap()[i]->teamNum = BLUETEAM;
						PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(IDLE);
						break;
					}
				}
				for (int i = 0; i < 2; ++i) {
					PLAYER->GetOtherPlayerMap()[i]->SetClientNum(paket->ids[i + 2]);
					PLAYER->GetOtherPlayerMap()[i]->NumberByPos(paket->posN[i + 2]);
					PLAYER->GetOtherPlayerMap()[i]->SetCharacterType((E_CHARACTERTYPE)paket->avatar[i + 2]);
					PLAYER->GetOtherPlayerMap()[i]->teamNum = REDTEAM;
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(IDLE);

				}
			}
			else {
				for (int i = 2; i < 4; ++i) {
					if (myid == paket->ids[i]) {
						PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
						PLAYER->GetPlayer()->SetClientNum(myid);
						PLAYER->GetPlayer()->NumberByPos(paket->posN[i]);
						PLAYER->GetPlayer()->teamNum = REDTEAM;
						PLAYER->GetPlayer()->SetPlayerState(IDLE);
					}
				}

				for (int i = 2; i < 4; ++i) {
					if (myid != paket->ids[i]) {
						PLAYER->GetTeamPlayerMap()[0]->SetClientNum(paket->ids[i]);
						PLAYER->GetTeamPlayerMap()[0]->NumberByPos(paket->posN[i]);
						PLAYER->GetTeamPlayerMap()[0]->SetCharacterType((E_CHARACTERTYPE)paket->avatar[i]);
						PLAYER->GetTeamPlayerMap()[0]->teamNum = REDTEAM;
						PLAYER->GetTeamPlayerMap()[0]->SetPlayerState(IDLE);
						break;
					}
				}


				for (int i = 0; i < 2; ++i) {
					PLAYER->GetOtherPlayerMap()[i]->SetClientNum(paket->ids[i]);
					PLAYER->GetOtherPlayerMap()[i]->NumberByPos(paket->posN[i]);
					PLAYER->GetOtherPlayerMap()[i]->SetCharacterType((E_CHARACTERTYPE)paket->avatar[i]);
					PLAYER->GetOtherPlayerMap()[i]->teamNum = BLUETEAM;
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(IDLE);
				}

			}
		}
		PLAYER->GetPlayer()->SetOOBB(PLAYER->GetPlayer()->GetPosition(), XMFLOAT3(25, 10, 25), XMFLOAT4(0, 0, 0, 1));
		CNetCGameFramework->ChangePlayerCharacter();

		PLAYER->GetPlayer()->m_match = true;
		CNetCGameFramework->SetCamera(PLAYER->GetPlayer()->GetCamera());
		break;
	}

	case SC_MOVE_PLAYER:
	{
		sc_packet_move *pkt = reinterpret_cast<sc_packet_move *>(ptr);

		XMFLOAT3 xmf3Shift = XMFLOAT3(0.0f, 0.0f, 0.0f);
		xmf3Shift = XMFLOAT3(pkt->posX, pkt->posY, pkt->posZ);

		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPosition(xmf3Shift);
			PLAYER->GetPlayer()->SetPlayerState(RUN);
			break;
		}
		else {
			for (int i = 0; i < 2; ++i) {
				if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetOtherPlayerMap()[i]->SetPosition(xmf3Shift);
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(RUN);
					break;
				}
				else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetTeamPlayerMap()[i]->SetPosition(xmf3Shift);
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(RUN);
					break;
				}
			}
		}
		break;
	}
	case SC_VECTOR_INFO:
	{
		sc_packet_vector *pkt = reinterpret_cast<sc_packet_vector *>(ptr);

		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetLookV(XMFLOAT3(pkt->LposX, pkt->LposY, pkt->LposZ));
			PLAYER->GetPlayer()->SetRightV(XMFLOAT3(pkt->RposX, pkt->RposY, pkt->RposZ));
			break;
		}
		else {
			for (int i = 0; i < 2; ++i) {
				if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetOtherPlayerMap()[i]->SetLookV(XMFLOAT3(pkt->LposX, pkt->LposY, pkt->LposZ));
					PLAYER->GetOtherPlayerMap()[i]->SetRightV(XMFLOAT3(pkt->RposX, pkt->RposY, pkt->RposZ));
					break;
				}
				else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetTeamPlayerMap()[i]->SetLookV(XMFLOAT3(pkt->LposX, pkt->LposY, pkt->LposZ));
					PLAYER->GetTeamPlayerMap()[i]->SetRightV(XMFLOAT3(pkt->RposX, pkt->RposY, pkt->RposZ));
					break;
				}
			}
		}
		break;
	}
	case SC_COLLISION:
	{
		sc_packet_collision *pkt = reinterpret_cast<sc_packet_collision *>(ptr);

		//어깨빵 시스템 OFF - 사용이 필요하면 서버에서 ON
		if (PLAYER->GetPlayer()->GetClientNum() == myid) {
			if (pkt->otherid == myid) { //내가가서 충돌
				PLAYER->GetPlayer()->SetPlayerState(PlayerState::STUN);
				break;
			}
			for (int i = 0; i < 2; ++i) {
				if (pkt->otherid == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
					break;
				}
				else if (pkt->otherid == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
					break;
				}
			}
		}
		break;
	}
	case SC_KEY_INFO:
	{
		sc_packet_key *pkt = reinterpret_cast<sc_packet_key *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			if (pkt->jump == true) {
				PLAYER->GetPlayer()->SetPlayerState(JUMP);
				PLAYER->GetPlayer()->SetJumpPower(450.0f);
				break;
			}
			if (pkt->attack == true) {
				PLAYER->GetPlayer()->SetPlayerState(ATTACK);
				break;
			}
			if (pkt->skill == true) {
				PLAYER->GetPlayer()->SetPlayerState(ATTACK_3);
				break;
			}

		}
		for (int i = 0; i < 2; ++i) {
			if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {

				if (pkt->jump == true) {
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(JUMP);
					PLAYER->GetOtherPlayerMap()[i]->SetJumpPower(450.0f);
					break;
				}
				if (pkt->attack == true) {
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(ATTACK);
					break;
					if (pkt->skill == true) {
						PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(ATTACK_3);
						break;
					}
				}
			}
			else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
				if (pkt->jump == true) {
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(JUMP);
					PLAYER->GetTeamPlayerMap()[i]->SetJumpPower(450.0f);
					break;
				}
				if (pkt->attack == true) {
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(ATTACK);
					break;
				}
				if (pkt->skill == true) {
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(ATTACK_3);
					break;
				}
			}
		}
		break;
	}
	case SC_ATTACK_INFO: {
		sc_packet_attack *pkt = reinterpret_cast<sc_packet_attack *>(ptr);
		if (pkt->hp >= 0) {
			if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
				PLAYER->GetPlayer()->SetPlayerState(PlayerState::STUN);
				PLAYER->GetPlayer()->SetHp(pkt->hp);
				break;
			}
			for (int i = 0; i < 2; ++i) {
				if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
					PLAYER->GetOtherPlayerMap()[i]->SetHp(pkt->hp);
					break;
				}
				else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
					PLAYER->GetTeamPlayerMap()[i]->SetHp(pkt->hp);
					break;

				}
			}
		}
		break;
	}
	case SC_LOBBY_IN:
	{
		if (CNetCGameFramework->m_ready == true) {
			sc_packet_lobby *pkt = reinterpret_cast<sc_packet_lobby *>(ptr);

			SCENEMANAGER->SetScene(MENUSCENE);
			PLAYER->GetPlayer()->m_match = false;
			PLAYER->GetPlayer()->SetPlayerState(IDLE);
			PLAYER->GetPlayer()->SetClientNum(-1);
			PLAYER->GetPlayer()->NumberByPos(SPAWN);
			PLAYER->GetPlayer()->SetHp(8);
			for (int i = 0; i < 2; ++i) {
				PLAYER->GetOtherPlayerMap()[i]->m_match = false;
				PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::IDLE);
				PLAYER->GetTeamPlayerMap()[i]->m_match = false;
				PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(IDLE);
				PLAYER->GetTeamPlayerMap()[i]->SetClientNum(-1);
				PLAYER->GetOtherPlayerMap()[i]->SetClientNum(-1);
				PLAYER->GetTeamPlayerMap()[i]->NumberByPos(SPAWN);
				PLAYER->GetOtherPlayerMap()[i]->NumberByPos(SPAWN);
				PLAYER->GetTeamPlayerMap()[i]->SetHp(8);
				PLAYER->GetOtherPlayerMap()[i]->SetHp(8);
			}
			CNetCGameFramework->m_ready = false;

		}
		break;
	}
	case SC_RESULT_INFO:
	{
		sc_packet_result *pkt = reinterpret_cast<sc_packet_result *>(ptr);
		if (pkt->result == true) {
			PLAYER->GetPlayer()->SetPlayerState(SAD);
			for (int i = 0; i < 2; ++i) {
				PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(SAD);
				PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::SAD);
			}
		}
		else {
			PLAYER->GetPlayer()->SetPlayerState(HAPPY);
			for (int i = 0; i < 2; ++i) {
				PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(HAPPY);
				PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::HAPPY);
			}
		}
		break;
	}
	case SC_DEATH:
	{
		sc_packet_death *pkt = reinterpret_cast<sc_packet_death *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPlayerState(DEATH);
		}
		for (int i = 0; i < 2; ++i) {
			if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
				PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::DEATH);
				break;
			}
			else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
				PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(PlayerState::DEATH);
				break;
			}
		}
		break;
	}
	case SC_CHAT:
	{
		//SetConsoleOutputCP(65001);
		sc_packet_chat *pkt = reinterpret_cast<sc_packet_chat *>(ptr);
		cout <<"cnet- " << pkt->chat <<endl;
		//cout << sizeof(pkt->chat) << endl;
		CHATMANAGER->TextChange(pkt->chat);
		//CHATMANAGER->InputChatting(pkt->chat, pkt->cSize);
		ZeroMemory(packet_buffer,sizeof(packet_buffer));
		
		break;
	}
	case SC_CLOCK:
	{
		sc_packet_clock *pkt = reinterpret_cast<sc_packet_clock *>(ptr);
		//여기에 들어오면 1초가 지나서 들어온것
		//m_time++; 
		if (m_skilCheck == true) {
			m_skillTime--;
			if (m_skillTime == 0)
				m_skilCheck = false;
		}
		break;
	}
	case SC_DONA:
	{
		sc_packet_dona *pkt = reinterpret_cast<sc_packet_dona *>(ptr);
		cout << "도네이션이 들어옴" << endl;
		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}
void CNetWork::MatchPkt()
{
	cs_packet_matching* pkt = reinterpret_cast<cs_packet_matching*>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_MATCHING_PLAYER;
	pkt->avatar = PLAYER->GetPlayer()->GetCharacterType();
	pkt->map = map;
	pkt->mod = mod;

	SendPacket();
}
void CNetWork::StatePkt(DWORD state)
{
	cs_packet_move_state *pkt = reinterpret_cast<cs_packet_move_state *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_MOVE_STATE_INFO;
	pkt->state = state;

	SendPacket();
}
void CNetWork::RotePkt(float y)
{
	cs_packet_rote_state *pkt = reinterpret_cast<cs_packet_rote_state *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_ROTE_STATE_INFO;
	pkt->y = y;

	SendPacket();
}

void CNetWork::KeyPkt(bool jump, bool attack, bool skill)
{
	cs_packet_key *pkt = reinterpret_cast<cs_packet_key *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_KEY_INFO;
	pkt->jump = jump;
	pkt->attack = attack;
	pkt->skill = skill;
	SendPacket();

}
void CNetWork::LobbyPkt(bool out)
{
	cs_packet_lobby_out *pkt = reinterpret_cast<cs_packet_lobby_out *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_LOBBY_OUT;

	SendPacket();
}
