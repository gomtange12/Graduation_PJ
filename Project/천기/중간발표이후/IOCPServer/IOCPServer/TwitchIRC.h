#pragma once
#include <string>
class TwitchIRC
{
private:

	//thread *autoPingThread; //Ÿ�̸� ������� �ص��ɵ�

	std::string serverAddr; 

	int serverPort; //6667

	std::string connectedChannel;

	SOCKET IRCsocket;

	unsigned char* buffer;

	std::string response;
public:
	TwitchIRC();
	~TwitchIRC();
};

TwitchIRC::TwitchIRC()
{
}

TwitchIRC::~TwitchIRC()
{
}