#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "Player.h"
#include "GameFramework.h"

#include "..\..\IOCPServer\IOCPServer\Protocol.h"


//#define IP_ADDR "192.168.22.79"
#define IP_ADDR "127.0.0.1"

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
	unsigned char	packet_buffer[MAX_BUFFER];

	DWORD	in_packet_size = 0;
	int		saved_packet_size = 0;

	bool firstCheck;

	CGameFramework*			CNetCGameFramework;
	int		m_time;
	bool	m_isCheckPkt  =  false;
public:
	int GetTime() { return m_time; }
	ModNumber mod = SQUAD;
	SceneState map = PLAYGROUNDMAP;
	int myid;

	CNetWork();
	~CNetWork();

	void SetGameFrameWork(CGameFramework* CGFramwork) { CNetCGameFramework = CGFramwork; };
	bool GetCheckPkt() { return m_isCheckPkt; }
	
	bool GetFirstCheck() const { return firstCheck; }
	void MakeServer(const HWND& hWnd);
	void SendPacket();
	void ReadPacket(SOCKET sock);
	void ProcessPacket(unsigned char *ptr);
	void MatchPkt();
	void StatePkt(DWORD state);
	void RotePkt(float y);
	
	void KeyPkt(bool jump, bool attack, bool skill);
	void LobbyPkt(bool out);
};
