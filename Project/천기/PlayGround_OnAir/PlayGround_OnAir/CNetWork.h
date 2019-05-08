#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "Player.h"
#include "GameFramework.h"
#include "..\..\IOCPServer\IOCPServer\Protocol.h"
//#include "..\..\õ��\IOCPServer\IOCPServer\Protocol.h"

class CNetWork : public CSingleTonBase<CNetWork>
{
private:
	//Server
	WSADATA	wsadata;
	SOCKET  g_mysocket;
	SOCKADDR_IN ServerAddr;

	WSABUF	send_wsabuf;
	char 	send_buffer[MAX_BUFFER];
	WSABUF	recv_wsabuf;
	char	recv_buffer[MAX_BUFFER];
	char	packet_buffer[MAX_BUFFER];

	DWORD	in_packet_size = 0;
	int		saved_packet_size = 0;

	bool firstCheck;

	CGameFramework*			CNetCGameFramework;

public:
	int myid;

	CNetWork();
	~CNetWork();

	void SetGameFrameWork(CGameFramework* CGFramwork) { CNetCGameFramework = CGFramwork; };

	bool GetFirstCheck() const { return firstCheck; }
	void MakeServer(const HWND& hWnd);
	void SendPacket();
	void ReadPacket(SOCKET sock);
	void ProcessPacket(char *ptr);
	void MatchPkt();
	void StatePkt(DWORD state);
	void RotePkt(float y);
	void PosPkt(const XMFLOAT3& pos);
	void KeyPkt(bool jump, bool attack, bool skill);
};
