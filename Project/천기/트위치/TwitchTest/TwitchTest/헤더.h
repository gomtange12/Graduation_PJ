#pragma once

//#include "..\..\IOCPServer\IOCPServer\Protocol.h"
#define MAX_BUFFER		1024
class TwitchIRC
{
public:
	TwitchIRC();
	~TwitchIRC();
	void Init();
	void InitSend();
	void Run();
	//void ProcessPacket();
	void stripMessage(std::string incoming, std::string &username, std::string &message);

private:
	WSADATA wsaData;
	int iResult;
	DWORD dwError;
	SOCKET sock;
	struct hostent *remoteHost;
	SOCKADDR_IN ServerAddr;
	char host_name[19] = "irc.chat.twitch.tv";
	unsigned char	packet_buffer[MAX_BUFFER];
	char recv_buffer[MAX_BUFFER];
	std::string  response;
	DWORD	in_packet_size = 0;
	int		saved_packet_size = 0;
	
};