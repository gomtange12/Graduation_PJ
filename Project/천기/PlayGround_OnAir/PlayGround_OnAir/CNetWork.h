#pragma once
#include "..\..\IOCPServer\IOCPServer\Protocol.h"

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

	void MakeServer();
	void SendPacket();
	void ReadPacket();
	void ProcessPacket(char *ptr);
	void ScenePacket(WORD num);
	void MatchPacket();
};
