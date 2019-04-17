#include "stdafx.h"
#include "CNetWork.h"
#include "CSceneManager.h"

CNetWork::CNetWork()
{
}

CNetWork::~CNetWork()
{
}

void CNetWork::MakeServer()
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

	//WSAAsyncSelect(g_mysocket, hMainWnd, WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = MAX_BUFFER;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = MAX_BUFFER;

}
void CNetWork::SendPacket()
{
	DWORD iobyte;
	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			printf("¿À·ù");
	}
}
void CNetWork::ReadPacket() 
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(g_mysocket, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
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
	}
	case SC_SCENE:
	{
		sc_packet_scene *paket = reinterpret_cast<sc_packet_scene *>(ptr);
		SCENEMANAGER->SetScene(static_cast<SceneState>(paket->sceneNum));
	}
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *my_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = my_packet->id;

		if (id == g_myid) {

			//PLAYER->GetPlayer()-
			/*player.x = my_packet->x;
			player.y = my_packet->y;
			player.attr |= 16;*/
		}
		else if (id < MAX_USER) {
			//skelaton[id].x = my_packet->x;
			//skelaton[id].y = my_packet->y;
			//skelaton[id].attr |= 16;
		}
		else {
			//npc[id - NPC_START].x = my_packet->x;
			//npc[id - NPC_START].y = my_packet->y;
			//npc[id - NPC_START].attr |= BOB_ATTR_VISIBLE;
		}
		break;
	}
	case SC_MOVE_PLAYER:
	{
		sc_packet_pos *my_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		int other_id = my_packet->id;
		//if (other_id == g_myid) {
		//	g_left_x = my_packet->X_POS - 4;
		//	g_top_y = my_packet->Y_POS - 4;
		//	player.x = my_packet->X_POS;
		//	player.y = my_packet->Y_POS;
		//}
		//else if (other_id < MAX_USER) {
		//	skelaton[other_id].x = my_packet->X_POS;
		//	skelaton[other_id].y = my_packet->Y_POS;
		//}
		//else {
		//	//npc[other_id - NPC_START].x = my_packet->x;
		//	//npc[other_id - NPC_START].y = my_packet->y;
		//}
		break;
	}

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;
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
		sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			wcsncpy_s(player.message, my_packet->message, 256);
			player.message_time = GetTickCount();
		}
		else if (other_id < NPC_START) {
			wcsncpy_s(skelaton[other_id].message, my_packet->message, 256);
			skelaton[other_id].message_time = GetTickCount();
		}
		else {
			wcsncpy_s(npc[other_id - NPC_START].message, my_packet->message, 256);
			npc[other_id - NPC_START].message_time = GetTickCount();
		}
		break;

	} */
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void CNetWork::ScenePacket(WORD num)
{
	sc_packet_scene *my_packet = reinterpret_cast<sc_packet_scene *>(send_buffer);
	send_wsabuf.len = sizeof(my_packet);
	my_packet->size = sizeof(my_packet);
	my_packet->type = SC_SCENE;
;
	my_packet->sceneNum = num;
	
	SendPacket();
}

void CNetWork::MatchPacket() 
{
	sc_packet_matching *my_packet = reinterpret_cast<sc_packet_matching *>(send_buffer);
	send_wsabuf.len = sizeof(my_packet);
	my_packet->size = sizeof(my_packet);
	my_packet->type = SC_MATCHING_PLAYER;
	//my_packet->charac =
	//my_packet->map =
	//my_packet->mod =

	SendPacket();
}