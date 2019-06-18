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
	
		if (paket->avatar == A) {
			PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
			PLAYER->GetPlayer()->SetClientNum(myid);
			PLAYER->GetOtherPlayer()->SetClientNum(paket->ids);
			PLAYER->GetPlayer()->m_match = true;
			
			CNetCGameFramework->SetCamera(PLAYER->GetPlayer()->GetCamera());
		}
		else if(paket->avatar == B){
			PLAYER->GetPlayer()->SetRoomNum(paket->roomNum);
			PLAYER->GetOtherPlayer()->SetClientNum(myid);
			PLAYER->GetPlayer()->SetClientNum(paket->ids);
			PLAYER->GetOtherPlayer()->m_match = true;
			
			CNetCGameFramework->SetCamera(PLAYER->GetOtherPlayer()->GetCamera());
			
		}

		
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
		
		XMFLOAT3 xmf3Shift = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float fDistance = 40.0f;

		if (PLAYER->GetPlayer()->GetClientNum() == myid) {
			if (pkt->id == myid) { //내가가서 충돌
				PLAYER->GetPlayer()->SetPosition(Vector3::Add(PLAYER->GetPlayer()->GetPosition(), PLAYER->GetPlayer()->GetLookVector(), -fDistance));
			}
			else { //상대가와서 충돌
				PLAYER->GetOtherPlayer()->SetPosition(Vector3::Add(PLAYER->GetOtherPlayer()->GetPosition(), PLAYER->GetOtherPlayer()->GetLookVector(), -fDistance));
			}
		}
		else {
			if (pkt->id == myid) {
				PLAYER->GetOtherPlayer()->SetPosition(Vector3::Add(PLAYER->GetOtherPlayer()->GetPosition(), PLAYER->GetOtherPlayer()->GetLookVector(), -fDistance));
			}
			else {
				PLAYER->GetPlayer()->SetPosition(Vector3::Add(PLAYER->GetPlayer()->GetPosition(), PLAYER->GetPlayer()->GetLookVector(), -fDistance));

			}
		}
	
		PLAYER->GetPlayer()->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		PLAYER->GetOtherPlayer()->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		PLAYER->GetPlayer()->SetPlayerState(PlayerState::STUN);
		PLAYER->GetOtherPlayer()->SetPlayerState(PlayerState::STUN);
		break;
	}
	case SC_KEY_INFO: 
	{
		sc_packet_key *pkt = reinterpret_cast<sc_packet_key *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			if (pkt->jump == true) {
				PLAYER->GetPlayer()->SetPlayerState(JUMP);
				PLAYER->GetPlayer()->SetJumpPower(450.0f);
			}
			if (pkt->attack == true) 
				PLAYER->GetPlayer()->SetPlayerState(ATTACK);

		}
		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum()) {
			if (pkt->jump == true) {
				PLAYER->GetOtherPlayer()->SetPlayerState(JUMP);
				PLAYER->GetOtherPlayer()->SetJumpPower(450.0f);
			}
			if (pkt->attack == true) 
				PLAYER->GetOtherPlayer()->SetPlayerState(ATTACK);
		}
		break;
	}
	case SC_ATTACK_INFO: {
		sc_packet_attack *pkt = reinterpret_cast<sc_packet_attack *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPlayerState(PlayerState::STUN);
		}
		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum()) {
			PLAYER->GetOtherPlayer()->SetPlayerState(PlayerState::STUN);
		}
		break;
	}
	case SC_LOBBY_IN:
	{
		sc_packet_lobby *pkt = reinterpret_cast<sc_packet_lobby *>(ptr);

		if (pkt->out == true) {
			SCENEMANAGER->SetScene(MENUSCENE);
			PLAYER->GetPlayer()->m_match = false;
			PLAYER->GetOtherPlayer()->m_match = false;
			PLAYER->GetPlayer()->SetPosition(XMFLOAT3(2560, 10, 1745));
			PLAYER->GetPlayer()->SetOOBB(PLAYER->GetPlayer()->GetPosition(), XMFLOAT3(7, 10, 7), XMFLOAT4(0, 0, 0, 1));
			PLAYER->GetOtherPlayer()->SetPosition(XMFLOAT3(440.0f, 50, 1745));
			PLAYER->GetOtherPlayer()->SetOOBB(PLAYER->GetOtherPlayer()->GetPosition(), XMFLOAT3(7, 10, 7), XMFLOAT4(0, 0, 0, 1));
			CNetCGameFramework->m_ready = false;
		}
		break;
	}
	case SC_RESULT_INFO: 
	{
		sc_packet_result *pkt = reinterpret_cast<sc_packet_result *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPlayerState(SAD);
			PLAYER->GetOtherPlayer()->SetPlayerState(HAPPY);
			
			
		}
		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum()) {
			PLAYER->GetOtherPlayer()->SetPlayerState(SAD);
			PLAYER->GetPlayer()->SetPlayerState(HAPPY);
			
		}
		break;
	}
	case SC_ALL_POS:
	{
		sc_packet_allpos *pkt = reinterpret_cast<sc_packet_allpos *>(ptr);
		if (pkt->id == PLAYER->GetPlayer()->GetClientNum()) {
			PLAYER->GetOtherPlayer()->SetPosition(XMFLOAT3(pkt->posX, PLAYER->GetOtherPlayer()->GetPosition().y,pkt->posZ));

			
		}
		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum()) {
			PLAYER->GetPlayer()->SetPosition(XMFLOAT3(pkt->posX, PLAYER->GetPlayer()->GetPosition().y, pkt->posZ));

		}
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *pkt = reinterpret_cast<sc_packet_remove_player *>(ptr);
	/*	if(pkt->id == PLAYER->GetPlayer()->GetClientNum()){

		}
		if (pkt->id == PLAYER->GetOtherPlayer()->GetClientNum()) {

		}*/
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
	pkt->mod = SOLO;

	SendPacket();
}
void CNetWork::StatePkt(DWORD state)
{
	cs_packet_move_state *pkt = reinterpret_cast<cs_packet_move_state*>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_MOVE_STATE_INFO;
	pkt->state = state;

	SendPacket();
}
void CNetWork::RotePkt(float y)
{
	cs_packet_rote_state *pkt = reinterpret_cast<cs_packet_rote_state*>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_ROTE_STATE_INFO;
	pkt->y = y;

	SendPacket();
}
void CNetWork::PosPkt(const XMFLOAT3& pos)
{
	cs_packet_pos *pkt = reinterpret_cast<cs_packet_pos *>(send_buffer);
	send_wsabuf.len = sizeof(pkt);
	pkt->size = sizeof(pkt);
	pkt->type = CS_POS_INFO;
	pkt->x = pos.x;
	//pkt->y = pos.y;
	int z =pos.z;
	pkt->z = z;
	cout << "pkt->z " << pkt->z << endl;

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
	pkt->out = out;

	SendPacket();
}
