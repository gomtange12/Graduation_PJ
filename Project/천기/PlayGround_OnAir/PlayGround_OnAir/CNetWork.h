#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>

#include "..\..\IOCPServer\IOCPServer\Protocol.h"
//#include "..\..\Ãµ±â\IOCPServer\IOCPServer\Protocol.h"

class CNetWork : public CSingleTonBase<CNetWork>
{
private:
	//Server
	SOCKET  g_mysocket;

	WSABUF	send_wsabuf;
	char 	send_buffer[MAX_BUFFER];
	WSABUF	recv_wsabuf;
	char	recv_buffer[MAX_BUFFER];
	char	packet_buffer[MAX_BUFFER];

	DWORD	in_packet_size = 0;
	int		saved_packet_size = 0;

	int		g_myid;
public:
	CNetWork();
	~CNetWork();

	void MakeServer(HWND hWnd);
	void SendPacket();
	void ReadPacket(SOCKET sock);
	void ProcessPacket(char *ptr);
	void MatchPkt();
	void StatePkt(DWORD state,float y);
};
