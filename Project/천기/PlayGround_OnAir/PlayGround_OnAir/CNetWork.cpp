#include "stdafx.h"
#include "CNetWork.h"
#include "CSceneManager.h"
#include "CPlayerManager.h"
#include "Player.h"
CNetWork::CNetWork()
{
}

CNetWork::~CNetWork()
{
}

void CNetWork::MakeServer(HWND hWnd)
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

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
void CNetWork::ProcessPacket(char *ptr) 
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case SC_LOGIN_OK:
	{
		sc_packet_login_ok *packet =
			reinterpret_cast<sc_packet_login_ok *>(ptr);
		g_myid = packet->id;
		break;
	}
	case SC_SCENE:
	{
		sc_packet_scene *paket = reinterpret_cast<sc_packet_scene *>(ptr);
		int roomNum = paket->roomNum; //플레이어가 가지고있으면 좋을듯
		SCENEMANAGER->SetScene(static_cast<SceneState>(paket->sceneNum));
		break;
	}
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *pkt = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = pkt->id;

		if (id == g_myid) {

			//PLAYER->GetPlayer()-
			/*player.x = pkt->x;
			player.y = pkt->y;
			player.attr |= 16;*/
		}
		else if (id < MAX_USER) {
			//skelaton[id].x = pkt->x;
			//skelaton[id].y = pkt->y;
			//skelaton[id].attr |= 16;
		}
		else {
			//npc[id - NPC_START].x = pkt->x;
			//npc[id - NPC_START].y = pkt->y;
			//npc[id - NPC_START].attr |= BOB_ATTR_VISIBLE;
		}
		break;
	}
	case SC_MOVE_PLAYER:
	{
		sc_packet_pos *pkt = reinterpret_cast<sc_packet_pos *>(ptr);
		int other_id = pkt->id;
		XMFLOAT3 xmf3Shift = XMFLOAT3(0.0f, 0.0f, 0.0f);
		xmf3Shift = XMFLOAT3(pkt->posX, pkt->posY, pkt->posZ);
		PLAYER->GetPlayer()->Move(xmf3Shift, pkt->velocity);
		//if (other_id == g_myid) {
		//	g_left_x = pkt->X_POS - 4;
		//	g_top_y = pkt->Y_POS - 4;
		//	player.x = pkt->X_POS;
		//	player.y = pkt->Y_POS;
		//}
		//else if (other_id < MAX_USER) {
		//	skelaton[other_id].x = pkt->X_POS;
		//	skelaton[other_id].y = pkt->Y_POS;
		//}
		//else {
		//	//npc[other_id - NPC_START].x = pkt->x;
		//	//npc[other_id - NPC_START].y = pkt->y;
		//}
		break;
	}

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *pkt = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = pkt->id;
		//if (other_id == g_myid) {
			//player.attr &= ~16;
		//}
		//else if (other_id < MAX_USER) {
			//skelaton[other_id].attr &= ~16;
		//}
		//else {
			//		npc[other_id - NPC_START].attr &= ~BOB_ATTR_VISIBLE;
		//}
		break;
	}
	/*
	case SC_CHAT:
	{
		sc_packet_chat *pkt = reinterpret_cast<sc_packet_chat *>(ptr);
		int other_id = pkt->id;
		if (other_id == g_myid) {
			wcsncpy_s(player.message, pkt->message, 256);
			player.message_time = GetTickCount();
		}
		else if (other_id < NPC_START) {
			wcsncpy_s(skelaton[other_id].message, pkt->message, 256);
			skelaton[other_id].message_time = GetTickCount();
		}
		else {
			wcsncpy_s(npc[other_id - NPC_START].message, pkt->message, 256);
			npc[other_id - NPC_START].message_time = GetTickCount();
		}
		break;

	} */
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}


void CNetWork::MatchPkt() 
{
	cs_packet_matching *pkt = reinterpret_cast<cs_packet_matching *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = SC_MATCHING_PLAYER;
	pkt->avatar = A;
	pkt->map = PLAYGROUND;
	pkt->mod = SOLO;

	SendPacket();
}
void CNetWork::StatePkt(DWORD state)
{
	cs_packet_state *pkt = reinterpret_cast<cs_packet_state *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = SC_STATE_INFO;
	pkt->state = state;

	pkt->LposX = PLAYER->GetPlayer()->GetLook().x;
	pkt->LposY = PLAYER->GetPlayer()->GetLook().y;
	pkt->LposZ = PLAYER->GetPlayer()->GetLook().z;

	pkt->RposX = PLAYER->GetPlayer()->GetRight().x;
	pkt->RposY = PLAYER->GetPlayer()->GetRight().y;
	pkt->RposZ = PLAYER->GetPlayer()->GetRight().z;

	SendPacket();
}