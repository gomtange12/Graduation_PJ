#include "pch.h"
#include "헤더.h"
#pragma execution_character_Set("utf-8")

void TwitchIRC::Init() 
{
	SetConsoleOutputCP(65001); //콘솔 인코딩

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	printf("Calling gethostbyname with %s\n", host_name);
	remoteHost = gethostbyname(host_name);
	if (!remoteHost)
	{
		printf("Failled to get %s(s) host by name... Error: %d\n", host_name, WSAGetLastError());
		exit(1);
	}
	printf("Successfully retrieved host by name...\n");
	int i = 0;
	while (remoteHost->h_addr_list[i] != 0)
	{
		ServerAddr.sin_addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
		printf("\tIP Address #%d: %s\n", i, inet_ntoa(ServerAddr.sin_addr));
	}
	char *ipaddr = inet_ntoa(ServerAddr.sin_addr);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("Socket == Invalid_Socket...\n");
		exit(1);
	}

	printf("Gathered All IP Adresses... Using %s\n", ipaddr);
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(6667);
	ServerAddr.sin_addr.s_addr = inet_addr(ipaddr);

	if (connect(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == 0)
	{
		printf("Connected..! \n");
	
		InitSend();

		Run();
	}
	else
	{
		printf("Failed to connect!\n");
	}

}

void TwitchIRC::Run() 
{
	int mret = 0;
	while (1) {
		DWORD dwTmp=0;
		
		//메세지 전송용
		
	/*	char text[1024] = { 0 };
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
		send(sock, text, strlen(text), 0);
		if (strcmp(recv_buffer, "exit") == 0) {
			send(sock, recv_buffer, (int)strlen(recv_buffer), 0);
			break;
		}*/
		
		mret = recv(sock, (char *)recv_buffer, sizeof(MAX_BUFFER) - 1, 0);
		if (mret == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;
		}
		response += string((char *)recv_buffer);
		fill_n(recv_buffer, sizeof(recv_buffer), NULL);;
		if (response.size() > 1 && response[response.size() - 2] == '\r' && response[response.size() - 1] == '\n') {
			
				string name, message;
				stripMessage(response, name, message);
				cout << "Chat: " << name << ": " << message << endl;
			
			
		}
	}

}
void TwitchIRC::stripMessage(string incoming, string &username, string &message) {
	string cName = "1k1000";
	SIZE_T nameBegin = incoming.find("display-name=") + 13;
	SIZE_T nameEnd = incoming.find(";", nameBegin);
	SIZE_T messageStart = incoming.find(cName + " :") + cName.size() + 2;
	//The correct format is :NAME!, test here
	username = incoming.substr(nameBegin, (nameEnd - nameBegin));
	//Fetch the message content
	if (messageStart != string::npos) {
		for (SIZE_T i = messageStart; i < incoming.size(); i++) {
			message.push_back(incoming[i]);
		}
	}
}
void TwitchIRC::InitSend() {
	send(sock, "PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r\r\n", strlen("PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r\r") + 1, 0); //토큰?
	send(sock, "NICK blqblq\r\n", strlen("NICK blqblq\r") + 1, 0);
	send(sock, "USER blqblq\r\n", strlen("USER blqblq\r") + 1, 0);

	send(sock, "CAP REQ :twitch.tv/commands\r\n", strlen("CAP REQ :twitch.tv/commands\r") + 1, 0);
	send(sock, "CAP REQ :twitch.tv/membership\r\n", strlen("CAP REQ :twitch.tv/membership\r") + 1, 0);
	send(sock, "CAP REQ :twitch.tv/tags\r\n", strlen("CAP REQ :twitch.tv/tags\r") + 1, 0);

	send(sock, "JOIN #1k1000\r\n", strlen("JOIN #1k1000\r") + 1, 0);
}
//void TwitchIRC::setNonBlocking(const bool status) {
//	UL block = status;
//	S32 opt = ioctlsocket(sock, FIONBIO, &block);
//	if (opt) {
//		cout << "Socket::setNonBlocking(): Failed with error " << WSAGetLastError() << endl;
//	}
//}
TwitchIRC::TwitchIRC()
{
}

TwitchIRC::~TwitchIRC()
{
	closesocket(sock);
	WSACleanup();
}
