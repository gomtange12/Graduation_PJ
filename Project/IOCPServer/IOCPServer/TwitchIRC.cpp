#include "pch.h"
#include "Protocol.h"

#include <winsock2.h>
#include <string>
#include <fstream>
#include "TwitchIRC.h"
#include "PacketManager.h"
#include "Player.h"
#include <locale>
void TwitchIRC::Init()
{
	mythread = std::thread([&]() { TwitchIRC::Proc(); });
}
void TwitchIRC::Proc() {
	SetConsoleOutputCP(65001); //콘솔 인코딩
	
	char line[124];
	std::fstream f("config.txt", std::ios::in);
	//hostName, port, pass, nick, channel
	while (!f.eof()) {
		f.getline(line, 124);
		config.push_back(line);
		memset(&line[0], 0, sizeof(line));
	}
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	hostAdd = gethostbyname(config[0].c_str());

	int i = 0;
	while (hostAdd->h_addr_list[i] != 0)
	{
		ServerAddr.sin_addr.s_addr = *(u_long *)hostAdd->h_addr_list[i++];
	}
	char *ipaddr = inet_ntoa(ServerAddr.sin_addr);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(atoi(config[1].c_str()));
	ServerAddr.sin_addr.s_addr = inet_addr(ipaddr);

	if (connect(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == 0)
	{
		std::cout << "TwitchIRC Connected..! \n" << std::endl;

		InitSend();
		Run();
	}
	else
	{
		std::cout << "Failed to connect!\n" << std::endl;
	}
}

void TwitchIRC::Run()
{
	while (1) {
		DWORD dwTmp = 0;

		//메세지 전송용
	/*	char text[1024] = { 0 };
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
		send(sock, text, strlen(text), 0);
		if (strcmp(recv_buffer, "exit") == 0) {
			send(sock, recv_buffer, (int)strlen(recv_buffer), 0);
			break;
		}*/
		
		recv(sock, (char *)recv_buffer, sizeof(MAX_BUFFER) - 1, 0);
		pkt += std::string((char *)recv_buffer);
		ZeroMemory(&recv_buffer, sizeof(recv_buffer));
		
		if (pkt.size() > 1 && pkt[pkt.size() - 2] == '\r' && pkt[pkt.size() - 1] == '\n') {
			if (pkt.find("PRIVMSG") !=  std::string::npos) {
				std::string name, message;
				stripMessage(pkt, name, message);
				
				
				std::cout << "Chat: " << chat << std::endl;
				pkt.resize(0);
			}
			if (pkt.find("PING") != std::string::npos) {
				send(sock, "PONG :tmi.twitch.tv\r\n", strlen("PONG :tmi.twitch.tv\r") + 1, 0);
			}
		}
		
	}

}
void TwitchIRC::stripMessage(std::string incoming, std::string &username, std::string &message) {
	
	if (first == false) {
		std::string cName = config[4];
		SIZE_T nameBegin = incoming.find("display-name=") + 13;
		SIZE_T nameEnd = incoming.find(";", nameBegin);
		SIZE_T messageStart = incoming.find(cName + " :") + cName.size() + 2;

		username = incoming.substr(nameBegin, (nameEnd - nameBegin));


		if (messageStart != std::string::npos) {
			for (SIZE_T i = messageStart; i < incoming.size(); i++) {
				message.push_back(incoming[i]);
			}
		}
		chat = std::string(username + " : " + message + " \r");
		
		PACKETMANAGER->TwitchChat(chat);
		
		SIZE_T dona = incoming.find("donation");
		if (dona == 278) {//278이면 도네이션보낸것
			PACKETMANAGER->DonaPacket();
			std::cout << "donation recv" << std::endl;
		}
	
	}
	first = false;
}
void TwitchIRC::InitSend() {
	const std::string pS = std::string("PASS " + config[2] + "\r\n");
	sendCommand(pS.c_str());
	const std::string nS = std::string("NICK " + config[3] + "\r\n");
	sendCommand(nS.c_str());

	//권한 부여
	send(sock, "CAP REQ :twitch.tv/commands\r\n", strlen("CAP REQ :twitch.tv/commands\r") + 1, 0);
	send(sock, "CAP REQ :twitch.tv/membership\r\n", strlen("CAP REQ :twitch.tv/membership\r") + 1, 0);
	send(sock, "CAP REQ :twitch.tv/tags\r\n", strlen("CAP REQ :twitch.tv/tags\r") + 1, 0);


	const std::string jS = std::string("JOIN " + config[4] + "\r\n");
	sendCommand(jS.c_str());

}
void TwitchIRC::sendCommand(const char* command) {
	send(sock, command, strlen(command), 0);
}

TwitchIRC::TwitchIRC()
{
}

TwitchIRC::~TwitchIRC()
{
	closesocket(sock);
	WSACleanup();
}
