#include "stdafx.h"
#include "CNetWork.h"
#include "CSceneManager.h"
#include "CPlayerManager.h"



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

		//캐릭터 설정 해줘야함 paket->avatar

		//솔로모드면
		//if (mod == SOLO) {
		/*	for (int i = 0; i < 2; ++i) {
				if (myid == paket->ids[i]) {
					PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
					PLAYER->GetPlayer()->SetClientNum(myid);
					PLAYER->GetPlayer()->NumberByPos(paket->posN[i]);
					PLAYER->GetPlayer()->m_match = true;
				}

				else {
					PLAYER->GetOtherPlayerMap()[0]->SetClientNum(paket->ids[i]);
					PLAYER->GetOtherPlayerMap()[0]->NumberByPos(paket->posN[i]);
				}
			}*/
			//}
			//else if (mod = SQUAD) {
				//팀모드면
		for (int i = 0; i < 2; ++i) {
			if (myid == paket->ids[i]) {
				PLAYER->GetPlayer()->teamNum = 0;

			}
		}
		if (PLAYER->GetPlayer()->teamNum == 0) {
			for (int i = 0; i < 2; ++i) {
				if (myid == paket->ids[i]) {
					PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
					PLAYER->GetPlayer()->SetClientNum(myid);
					PLAYER->GetPlayer()->NumberByPos(paket->posN[i]);

				}
			}

			for (int i = 0; i < 2; ++i) {
				if (myid != paket->ids[i]) {
					PLAYER->GetTeamPlayerMap()[i]->SetClientNum(paket->ids[i]);
					PLAYER->GetTeamPlayerMap()[i]->NumberByPos(paket->posN[i]);
				}
			}
			for (int i = 0; i < 2; ++i) {
				PLAYER->GetOtherPlayerMap()[i]->SetClientNum(paket->ids[i + 2]);
				PLAYER->GetOtherPlayerMap()[i]->NumberByPos(paket->posN[i + 2]);
			}
		}
		else {
			for (int i = 2; i < 4; ++i) {
				if (myid == paket->ids[i]) {
					PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
					PLAYER->GetPlayer()->SetClientNum(myid);
					PLAYER->GetPlayer()->NumberByPos(paket->posN[i]);
				}
			}

			for (int i = 2; i < 4; ++i) {
				if (myid != paket->ids[i]) {
					PLAYER->GetTeamPlayerMap()[0]->SetClientNum(paket->ids[i]);
					PLAYER->GetTeamPlayerMap()[0]->NumberByPos(paket->posN[i]);
				}
			}


			for (int i = 0; i < 2; ++i) {
				PLAYER->GetOtherPlayerMap()[i]->SetClientNum(paket->ids[i]);
				PLAYER->GetOtherPlayerMap()[i]->NumberByPos(paket->posN[i]);
			}

		}
		//	}		

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

		XMFLOAT3 xmf3Shift = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float fDistance = 5.25f;

		if (PLAYER->GetPlayer()->GetClientNum() == myid) {
			if (pkt->id == myid || pkt->otherid == myid) { //내가가서 충돌
				PLAYER->GetPlayer()->SetPosition(Vector3::Add(PLAYER->GetPlayer()->GetPosition(), PLAYER->GetPlayer()->GetLookVector(), -fDistance));
				PLAYER->GetPlayer()->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
				PLAYER->GetPlayer()->SetPlayerState(PlayerState::STUN);
				break;
			}
			for (int i = 0; i < 2; ++i) {
				if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum() || pkt->otherid == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetOtherPlayerMap()[i]->SetPosition(Vector3::Add(PLAYER->GetOtherPlayerMap()[i]->GetPosition(), PLAYER->GetOtherPlayerMap()[i]->GetLookVector(), -fDistance));
					PLAYER->GetOtherPlayerMap()[i]->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
					PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
					break;
				}
				else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum() || pkt->otherid == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetTeamPlayerMap()[i]->SetPosition(Vector3::Add(PLAYER->GetTeamPlayerMap()[i]->GetPosition(), PLAYER->GetTeamPlayerMap()[i]->GetLookVector(), -fDistance));
					PLAYER->GetTeamPlayerMap()[i]->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
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
			}
		}
		break;
	}
	case SC_ATTACK_INFO: {
		sc_packet_attack *pkt = reinterpret_cast<sc_packet_attack *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPlayerState(PlayerState::STUN);
			break;
		}
		for (int i = 0; i < 2; ++i) {
			if (pkt->id == PLAYER->GetOtherPlayerMap()[i]->GetClientNum()) {
				PLAYER->GetOtherPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
				break;
			}
			else if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
				if (pkt->id == PLAYER->GetTeamPlayerMap()[i]->GetClientNum()) {
					PLAYER->GetTeamPlayerMap()[i]->SetPlayerState(PlayerState::STUN);
					break;
				}
			}
		}
		break;
	}
	case SC_LOBBY_IN:
	{
		sc_packet_lobby *pkt = reinterpret_cast<sc_packet_lobby *>(ptr);

		SCENEMANAGER->SetScene(MENUSCENE);
		PLAYER->GetPlayer()->m_match = false;
		PLAYER->GetOtherPlayerMap()[0]->m_match = false;
		PLAYER->GetPlayer()->SetPosition(XMFLOAT3(2560, 10, 1745));
		PLAYER->GetPlayer()->SetOOBB(PLAYER->GetPlayer()->GetPosition(), XMFLOAT3(7, 10, 7), XMFLOAT4(0, 0, 0, 1));
		PLAYER->GetOtherPlayerMap()[0]->SetPosition(XMFLOAT3(440.0f, 50, 1745));
		PLAYER->GetOtherPlayerMap()[0]->SetOOBB(PLAYER->GetOtherPlayerMap()[0]->GetPosition(), XMFLOAT3(7, 10, 7), XMFLOAT4(0, 0, 0, 1));
		CNetCGameFramework->m_ready = false;


		break;
	}
	case SC_RESULT_INFO:
	{
		sc_packet_result *pkt = reinterpret_cast<sc_packet_result *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPlayerState(SAD);
			PLAYER->GetOtherPlayerMap()[0]->SetPlayerState(HAPPY);


		}
		if (pkt->id == PLAYER->GetOtherPlayerMap()[0]->GetClientNum()) {
			PLAYER->GetOtherPlayerMap()[0]->SetPlayerState(SAD);
			PLAYER->GetPlayer()->SetPlayerState(HAPPY);

		}
		break;
	}


	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}
void CNetWork::MatchPkt()
{
	cs_packet_matching *pkt = reinterpret_cast<cs_packet_matching *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_MATCHING_PLAYER;
	//if (firstCheck == true || myid % 2 ==0) {
	//	pkt->avatar = A;
	//}
	//else {
	//	pkt->avatar = B;
	//}
	pkt->map = PLAYGROUNDMAP;
	pkt->mod = SQUAD;

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
