#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "Player.h"
//#include "..\..\IOCPServer\IOCPServer\Protocol.h"
#include "..\..\Ãµ±â\IOCPServer\IOCPServer\Protocol.h"

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

	int myid;
	
public:
	CNetWork();
	~CNetWork();

	void MakeServer(const HWND& hWnd);
	void SendPacket();
	void ReadPacket(SOCKET sock);
	void ProcessPacket(char *ptr);
	void MatchPkt();
	void StatePkt(DWORD state);
	void RotePkt(float y);

	
};
