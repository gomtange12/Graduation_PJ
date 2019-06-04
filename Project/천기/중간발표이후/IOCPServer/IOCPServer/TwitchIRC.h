#pragma once
#include "MyThread.h"


class TwitchIRC : public MyThread
{
private:
	WSADATA			 wsaData;
	SOCKET			 tw_sock;
	SOCKADDR_IN		 ServerAddr;
	struct hostent   *hostAdd;
	int				 iResult;
	DWORD			 dwError;
	unsigned char	 packet_buffer[MAX_BUFFER];
	char			 recv_buffer[MAX_BUFFER];
	std::string		 response;
	DWORD			 in_packet_size = 0;
	int				 saved_packet_size = 0;
	std::vector<std::string> config;
public:
	TwitchIRC();
	~TwitchIRC();

	virtual void Init();
	virtual void Proc();
	void Run();
	void stripMessage(std::string incoming, std::string &username, std::string &message);
	void InitSend();
	void sendCommand(const char* command);
	void setNonBlocking(const bool status);
	//void ProcessPacket();

};