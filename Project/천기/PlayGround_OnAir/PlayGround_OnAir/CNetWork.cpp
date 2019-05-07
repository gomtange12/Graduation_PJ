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
		myid = packet->id;
		firstCheck = packet->check;
		break;
	}
	case SC_SCENE:
	{
		sc_packet_scene *paket = reinterpret_cast<sc_packet_scene *>(ptr);
		SCENEMANAGER->SetScene(static_cast<SceneState>(paket->sceneNum));

		if (firstCheck == true) {
			PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
			PLAYER->GetOtherPlayer()->SetClientNum(paket->ids);
			PLAYER->GetPlayer()->SetClientNum(myid);
			CNetCGameFramework->SetCamera(PLAYER->GetPlayer()->GetCamera());
		}
		else if (firstCheck == false) {
			PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
			PLAYER->GetOtherPlayer()->SetClientNum(myid);
			PLAYER->GetPlayer()->SetClientNum(paket->ids);
			CNetCGameFramework->SetCamera(PLAYER->GetOtherPlayer()->GetCamera());
		}

		PLAYER->GetPlayer()->m_match = true;
		break;
	}
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *pkt = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = pkt->id;

		if (id == PLAYER->GetPlayer()->GetClientNum()) {


		}
		else if (id < MAX_USER) {

		}
		else {

		}
		break;
	}
	case SC_MOVE_PLAYER:
	{
		sc_packet_move *pkt = reinterpret_cast<sc_packet_move *>(ptr);

		XMFLOAT3 xmf3Shift = XMFLOAT3(0.0f, 0.0f, 0.0f);
		xmf3Shift = XMFLOAT3(pkt->posX, pkt->posY, pkt->posZ);

		if (pkt->id == PLAYER->GetPlayer()->GetClientNum())
			PLAYER->GetPlayer()->Move(xmf3Shift, pkt->velocity);

		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum())
			PLAYER->GetOtherPlayer()->Move(xmf3Shift, pkt->velocity);

		break;
	}
	case SC_VECTOR_INFO:
	{
		sc_packet_vector *pkt = reinterpret_cast<sc_packet_vector *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetLookV(XMFLOAT3(pkt->LposX, pkt->LposY, pkt->LposZ));
			PLAYER->GetPlayer()->SetRightV(XMFLOAT3(pkt->RposX, pkt->RposY, pkt->RposZ));
		}
		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum()) {
			PLAYER->GetOtherPlayer()->SetLookV(XMFLOAT3(pkt->LposX, pkt->LposY, pkt->LposZ));
			PLAYER->GetOtherPlayer()->SetRightV(XMFLOAT3(pkt->RposX, pkt->RposY, pkt->RposZ));
		}
		break;
	}
	case SC_COLLISION:
	{
		sc_packet_collision *pkt = reinterpret_cast<sc_packet_collision *>(ptr);
		PLAYER->GetPlayer()->SetPlayerCollision(pkt->check); //충돌체크값 bool변수
		if (firstCheck == true) {
			//PLAYER->GetPlayer()->SetPlayerCollision(pkt->check);
			//PLAYER->GetPlayer()->SetPosition(Vector3::Subtract(PLAYER->GetPlayer()->GetPrePosition(), XMFLOAT3(0.f, 0.f, 1.f)));
			cout << "아앜" << endl;
		}
		else {
			//PLAYER->GetOtherPlayer()->SetPlayerCollision(pkt->check);
			//PLAYER->GetOtherPlayer()->SetPosition(Vector3::Subtract(PLAYER->GetOtherPlayer()->GetPrePosition(), XMFLOAT3(0.f, 0.f, 1.f)));
			cout << "아앜22" << endl;
		}

		break;
	}
	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *pkt = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = pkt->id;

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
	pkt->avatar = A;
	pkt->map = PLAYGROUND;
	pkt->mod = SOLO;

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
void CNetWork::Pos(const XMFLOAT3& pos)
{
	cs_packet_pos *pkt = reinterpret_cast<cs_packet_pos *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_POS_INFO;
	pkt->x = pos.x;
	pkt->y = pos.y;
	pkt->z = pos.z;
	SendPacket();
}