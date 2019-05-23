#pragma once
#include <string>
class TwitchIRC
{
private:

	//thread *autoPingThread; //타이머 쓰레드로 해도될듯

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