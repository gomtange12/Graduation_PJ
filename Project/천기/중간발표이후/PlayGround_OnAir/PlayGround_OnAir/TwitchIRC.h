#pragma once
#include "MyThread.h"
#include "ObjManager.h"

class TwitchIRC : public MyThread
{
private:
	std::vector<std::string> config;

	WSADATA			 wsaData;
	SOCKET			 sock;
	SOCKADDR_IN		 ServerAddr;
	struct hostent   *hostAdd;

	char			 recv_buffer[MAX_BUFFER];
	std::string		 pkt;

	bool first = true;

public:
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();
	std::string chat;
	TwitchIRC();
	virtual ~TwitchIRC();

	virtual void Init();
	virtual void Proc();

	void Run();
	void stripMessage(std::string incoming, std::string &username, std::string &message);
	void InitSend();
	void sendCommand(const char* command);

};